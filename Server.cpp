#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include "Server.hpp"
#include "Command.hpp"

Server::Server(int port, string password) :  _command_controller(CommandController()), _password(password), _name(HOST), _port(port), _socket(-1)
{
	if ((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		handle_error("Server Socket error");

	memset(&_server_addr, 0, sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_addr.sin_port = htons(_port);

	if (bind(_socket, (sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
		handle_error("Server Bind error");
	
	if (listen(_socket, 5) == -1)
		handle_error("Server Listen error");
}

Server::~Server()
{
	cout << "[INFO][" << _getTimestamp() << "] Server cleaning all Channels...\n";
	for (size_t i = 0; i < _channels.size(); ++i)
		delete _channels[i];
	cout << "[INFO][" << _getTimestamp() << "] Server Successfully delete all Channels.\n";
	cout << "[INFO][" << _getTimestamp() << "] Server cleaning all Users...\n";
	for (size_t i = 0; i < _clients.size(); ++i)
		delete _clients[i];
	cout << "[INFO][" << _getTimestamp() << "] Server Successfully delete all Users.\n";
	cout << "[INFO][" << _getTimestamp() << "] Server Successfully Closed.\n";
}

void	Server::run()
{
	int kq;

	if ((kq = kqueue()) == -1)
		handle_error("Server Kqueue init error");

	this->_clients.push_back(new Bot(this));
	//bot client add
	change_events(_change_list, _socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	//debugging
	change_events(_change_list, STDIN_FILENO, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	cout << "[INFO][" << _getTimestamp() << "] Server Successfully started.\n";

	int new_events;
	struct kevent*	curr_event;

	while (1)
	{
		new_events = kevent(kq, &_change_list[0], _change_list.size(), _event_list, 8, NULL);
		if (new_events == -1)
			handle_error("Kqueue Event Error");

		_change_list.clear();
		for (int i = 0; i < new_events; ++i)
		{
			curr_event = &_event_list[i];

			if (curr_event->flags & EV_ERROR)
			{
				if ((int)curr_event->ident == _socket)
					handle_error("Server Socket Error");
				else
				{
					cerr << "[ERROR][" << _getTimestamp() << "] Client socket error.\n";
					disconnect_client(curr_event->ident);
				}
			}
			else if (curr_event->ident == STDIN_FILENO)
				debugger();
			else if (curr_event->filter == EVFILT_READ)
			{
				if ((int)curr_event->ident == _socket)
					bindClient();
				else if (getClient(curr_event->ident) != NULL)
					if (!getClient(curr_event->ident)->recv() || curr_event->flags & EV_EOF)
						disconnect_client(curr_event->ident);
			}
			else if (curr_event->filter == EVFILT_WRITE)
			{
				Client* cl = getClient(curr_event->ident);
				Command* cmd = this->_command_controller.makeCommand(*cl);
				if (cmd != NULL)
					cmd->execute(*this, *cl);
			}
		}
	}
}

void	Server::debugger()
{
	char buf[1024];
	size_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
	if (n > 0)
	{
		buf[n] = 0;
		string str(buf);
		istringstream	iss(str);
		string			cmd, token;

		iss >> cmd;
		iss >> token;
		//channel
		if (cmd == "showc")
		{
			Channel* ch = getChannel(token);
			if (ch != nullptr)
				cout << *ch;
			else
				cout << "[INFO][DEBUGGER] NO SUCH CHANNEL\n";
		}
		//user
		else if (cmd == "showu")
		{
			Client* cl = getClient(token);
			if (cl != nullptr)
				cout << *cl;
			else
				cout << "[INFO][DEBUGGER] NO SUCH CLIENT\n";
		}
		else if (cmd == "showallu")
		{
			for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
			{
				cout << (*it);
			}
		}
		else
			cout << "[INFO][DEBUGGER] UNKNOWN COMMAND\n";
	}
}

void	Server::change_events(vector<struct kevent>& change_list, uintptr_t ident
		, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
	change_list.push_back(temp_event);
}

void	Server::disconnect_client(int client_fd)
{
	Client* cl = getClient(client_fd);
	if (cl == NULL)
		return ;
	close(client_fd);
	cout << "[INFO][" << _getTimestamp() << "] Client Fd: " << client_fd << " disconnected\n";
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getSocketFd() == client_fd)
		{
			for (size_t j = 0; j < _channels.size(); ++j)
			{
				if (_channels[j]->checkUserInChannel(cl->getNickName()))
				{
					_channels[j]->leave(cl);
					if (_channels[j]->getUsers().size() == 0)
						deleteChannel(_channels[j]->getName());
				}
				if (_channels[j]->isInvited(cl->getNickName()))
					_channels[j]->accept(cl);
			}
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
	delete cl;
}

int		Server::bindClient()
{
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int so_client;
	if ((so_client = accept(_socket, (struct sockaddr*)&client_addr, &client_len)) == -1)
	{
		cerr << "[ERROR][" << _getTimestamp() << "] Client Bind(Accept) error. Failed to bind new Client.\n";
		return (0);
	}
	cout << "[INFO][" << _getTimestamp() << "][ACCEPT] New Client at " << so_client << "th fd\n";

	Client*	cl = new Client(so_client, static_cast<string>(inet_ntoa(client_addr.sin_addr)));

	_clients.push_back(cl);
	fcntl(so_client, F_SETFL, O_NONBLOCK);

	change_events(_change_list, so_client, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	change_events(_change_list, so_client, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	return (1);
}

void	Server::notify(string nick, string msg)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->checkUserInChannel(nick))
			_channels[i]->broadcast(msg, getClient(nick));
	}
}

int	Server::createChannel(string ch_name, Client* owner)
{
	if (getChannel(ch_name) != NULL)
	{
		cout << "[ERROR][" << _getTimestamp() << "] Channel " << ch_name << " is already exist.\n";
		return (0);
	}
	else
	{
		Channel* ch = new Channel(ch_name, owner);
		_channels.push_back(ch);
		return (1);
	}
}

void	Server::deleteChannel(string ch_name)
{
	Channel*	ch = getChannel(ch_name);
	if (ch == nullptr)
	{
		cout << "[ERROR][" << _getTimestamp() << "] " << ch_name << " Channel is not exist.\n";
		return ;
	}
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == ch_name)
		{
			_channels.erase(_channels.begin() + i);
			break ;
		}
	}
	delete ch;
}

string	Server::getPassword() const
{
	return _password;
}

string	Server::getServername() const
{
	return _name;
}

Client*	Server::getClient(int fd)
{
	vector<Client*>::const_iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (fd == (*it)->getSocketFd())
			return (*it);
		it++;
	}
	return NULL;
}

Client*	Server::getClient(string nick)
{
	vector<Client*>::const_iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (nick == (*it)->getNickName())
			return (*it);
		it++;
	}
	return NULL;
}

Channel*	Server::getChannel(string ch_name)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == ch_name)
			return _channels[i];
	}
	return NULL;
}

// test error -> exception
void	Server::handle_error(string err)
{
	cerr << "[ERROR][" << _getTimestamp() << "] " << err << "\n";
	exit(1);
}

string	Server::_getTimestamp()
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(now);
	
	// 스트림에 시간을 포맷팅하여 출력
	stringstream	ss;
	ss << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S");
	
	return ss.str();
}