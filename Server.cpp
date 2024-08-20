#include <iostream>
#include "Server.hpp"
#include "Command.hpp"

Server::Server(int port, string password) :  _command_controller(CommandController()), _password(password), _name(HOST), _port(port), _socket(-1)
{
	if ((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		handle_error("socket error");

	memset(&_server_addr, 0, sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_addr.sin_port = htons(_port);

	if (bind(_socket, (sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
		handle_error("bind error");
	
	if (listen(_socket, 5) == -1)
		handle_error("listen error");
}

Server::~Server()
{

}

void	Server::run()
{
	int kq;
	if ((kq = kqueue()) == -1)
		handle_error("kqueue error");

	map<int, string>		clients;

	change_events(_change_list, _socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	cout << "server started\n";

	int new_events;
	struct kevent*	curr_event;

	while (1)
	{
		new_events = kevent(kq, &_change_list[0], _change_list.size(), _event_list, 8, NULL);
		if (new_events == -1)
		{
			cerr << "kevent error\n";
			exit(1);
		}

		_change_list.clear();
		for (int i = 0; i < new_events; ++i)
		{
			curr_event = &_event_list[i];

			if (curr_event->flags & EV_ERROR)
			{
				if ((int)curr_event->ident == _socket)
				{
					cerr << "server socket error\n";
					exit(1);
				}
				else
				{
					cerr << "client socket error\n";
					disconnect_client(curr_event->ident, clients);
				}
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if ((int)curr_event->ident == _socket)
				{
					cout << "bind Client\n";
					bindClient();
				}
				else if (getClient(curr_event->ident)->getSocketFd() != -1)
				{
					getClient(curr_event->ident)->recv();
				}
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

void	Server::change_events(vector<struct kevent>& change_list, uintptr_t ident
		, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
	change_list.push_back(temp_event);
}

void	Server::disconnect_client(int client_fd, map<int, string>& clients)
{
	cout << "client disconnected: " << client_fd << endl;
	close(client_fd);
	clients.erase(client_fd);
}

int		Server::bindClient()
{
	struct sockaddr_in	client_addr;
	int so_client;
	if ((so_client = accept(_socket, (struct sockaddr*)&client_addr, 0)) == -1)
	{
		cerr << "accept error\n";
		return (0);
	}
	cout << "accept new client: " << so_client << "\n";
	_clients.push_back(Client(so_client, static_cast<string>(inet_ntoa(client_addr.sin_addr))));
	fcntl(so_client, F_SETFL, O_NONBLOCK);

	change_events(_change_list, so_client, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	change_events(_change_list, so_client, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	return (1);
}

int	Server::createChannel(string ch_name, Client* owner)
{
	if (getChannel(ch_name)->getName() != "EMPTY")
	{
		cout << "[ERROR] Channel " << ch_name << " is already exist.\n";
		return (0);
	}
	else
	{
		_channels.push_back(Channel(ch_name, owner));
		return (1);
	}
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
	vector<Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (fd == (*it).getSocketFd())
			return &(*it);
		it++;
	}
	cout << "cannot find " << fd << " user\n";
	//throw
	return NULL;
}

Client*	Server::getClient(string nick)
{
	vector<Client>::iterator it = _clients.begin();
	while (it != _clients.end())	
	{
		if (nick == (*it).getNickName())
			return &(*it);
		it++;
	}
	cout << "cannot find " << nick << " user\n";
	//throw
	return NULL;
}

Channel*	Server::getChannel(string ch_name)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i].getName() == ch_name)
			return &_channels[i];
	}
	return NULL;
}

// test error -> exception
void	handle_error(string err)
{
	cerr << err << "\n";
	exit(1);
}