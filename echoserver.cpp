#include <iostream>
#include <map>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <poll.h>

#define ADDR_FAMILY AF_INET
#define BUF_SIZE 512

using namespace std;

void change_events(vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
		uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
	change_list.push_back(temp_event);
}

int main(int argc, char * argv[])
{
	int so_server, server_port;
	//arguments check without password (with password needs to be equals 3)
	if (argc != 2 || !(server_port = atoi(argv[1])))
		return (1);

	// socket fd init

	//주소 구조체 init
	struct sockaddr_in	addr_server;

	//socket create
	if ((so_server = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		cerr << "socket error\n";
	
	memset(&addr_server, 0, sizeof(addr_server));
	// htonl, htos -> convert to big endian
	addr_server.sin_family = ADDR_FAMILY;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(atoi(argv[1]));

	//bind socket with addr
	if (bind(so_server, (sockaddr*)&addr_server, sizeof(addr_server)) == -1)
	{
		cerr << "bind error\n";
		exit(1);
	}

	//wait connection
	if (listen(so_server, 5) == -1)
	{
		cerr << "listen error\n";
		exit(1);
	}

	int kq;
	if ((kq = kqueue()) == -1)
	{
		cerr << "kqueue error\n";
		exit(1);
	}

	map<int, string>	clients;
	vector<struct kevent> change_list;
	struct kevent	event_list[8];
	
	change_events(change_list, so_server, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	cout << "Server Started\n";

	int new_events;
	struct kevent* curr_event;
	
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
				if ((int)curr_event->ident == so_server)
				{
					cerr << "server socket error\n";
					exit(1);
				}
				else
				{
					cerr << "client socket error\n";
					cout << "client disconnected: " << curr_event->ident << endl;
					close(curr_event->ident);
					clients.erase(curr_event->ident);
				}
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if ((int)curr_event->ident == so_server)
				{
					int so_client;
					if ((so_client = accept(so_server, 0, 0)) == -1)
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
							cout << "client disconnected: " << curr_event->ident << endl;
							close(curr_event->ident);
							clients.erase(curr_event->ident);
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
						if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(), clients[curr_event->ident].size())) == -1)
						{
							cerr << "client write error\n";
							cout << "client disconnected: " << curr_event->ident << endl;
							close(curr_event->ident);
							clients.erase(curr_event->ident);
						}
						else
							clients[curr_event->ident].clear();
					}
				}
			}
		}
	}
	return (0);
}
