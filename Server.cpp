#include "Server.hpp"

Server::Server(int port, string password) :_password(password), _port(port), _socket(-1)
{

}

Server::~Server()
{

}

void	Server::run()
{
	struct sockaddr_in	server_addr;

	if ((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		handle_error("socket error");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(_port);
	cout << "init" << "\n";

	if (bind(_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		handle_error("bind error");
	
	if (listen(_socket, 5) == -1)
		handle_error("listen error");
	
	int kq;
	if ((kq = kqueue()) == -1)
		handle_error("kqueue error");

	map<int, string>		clients;
	vector<struct kevent>	change_list;
	struct kevent			event_list[8];

	change_events(change_list, _socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	cout << "server started\n";

	int new_events;
	struct kevent*	curr_event;

	while (1)
	{
		new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
		if (new_events == -1)
		{
			cerr << "kevent error\n";
			exit(1);
		}

		change_list.clear();
		for (int i = 0; i < new_events; ++i)
		{
			curr_event = &event_list[i];

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
					int so_client;
					if ((so_client = accept(_socket, 0, 0)) == -1)
					{
						cerr << "accept error\n";
						exit(1);
					}
					cout << "accept new client: " << so_client << "\n";
					fcntl(so_client, F_SETFL, O_NONBLOCK);

					change_events(change_list, so_client, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					change_events(change_list, so_client, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					clients[so_client] = "";
				}
				else if (clients.find(curr_event->ident) != clients.end())
				{
					char buf[1024];
					int n = read(curr_event->ident, buf, sizeof(buf));

					if (n <= 0)
					{
						if (n < 0)
						{
							cerr << "client read error\n";
							disconnect_client(curr_event->ident, clients);
						}
					}
					else
					{
						buf[n] = 0;
						clients[curr_event->ident] += buf;
						cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << "\n";
					}
				}
			}
			else if (curr_event->filter == EVFILT_WRITE)
			{
				map<int, string>::iterator it = clients.find(curr_event->ident);
				if (it != clients.end())
				{
					if (clients[curr_event->ident] != "")
					{
						int n;
						cout << "write:" << clients[curr_event->ident] << "\n";
						if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(), clients[curr_event->ident].size())) == -1)
						{
							cerr << "client write error\n";
							disconnect_client(curr_event->ident, clients);
						}
						else
							clients[curr_event->ident].clear();
					}
				}
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

// test error -> exception
void	handle_error(string err)
{
	cerr << err << "\n";
	exit(1);
}