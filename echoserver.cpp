#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

#define ADDR_FAMILY AF_INET
#define BUF_SIZE 512

using namespace std;

int main(int argc, char * argv[])
{
	int so_server, so_client, server_port;
	//arguments check without password (with password needs to be equals 3)
	if (argc != 2 || !(server_port = atoi(argv[1])))
		return (1);

	// socket fd init

	//주소 구조체 init
	struct sockaddr_in	addr_server;
	struct sockaddr_in	addr_client;
	socklen_t			client_addr_size;

	//socket create
	if ((so_server = socket(ADDR_FAMILY, SOCK_STREAM, 0)) == -1)
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

	struct pollfd clients[512];
	for (int i = 0; i < 512; ++i)
		clients[i].fd = -1;

	clients[0].fd = so_server;
	clients[0].events = POLLIN;

	int max_i = 0, i = 0;
	
	while (1)
	{
		if (poll(clients, max_i + 1, 5000) <= 0)
			continue;
		
		if (clients[0].revents & POLLIN)
		{
			client_addr_size = sizeof(addr_client);
			so_client = accept(so_server, (sockaddr*)&addr_client, &client_addr_size);

			for (i = 1; i < 512; ++i)
			{
				if (clients[i].fd < 0)
				{
					clients[i].fd = so_client;
					clients[i].events = POLLIN;
					break;
				}
			}

			if (i == 512)
				cerr << "too many clients";

			if (i > max_i)
				max_i = i;
			cout << so_client << " fd connected\n";
			continue;
		}

		for (i = 1; i < 512; ++i)
		{
			if (clients[i].fd < 0)
				continue;

			if (clients[i].revents & POLLIN)
			{
				char	buf[BUF_SIZE];
				size_t	buf_len;

				memset(buf, 0, BUF_SIZE);

				if ((buf_len = recv(so_client, buf, BUF_SIZE, 0)) <= 0)
				{
					cout << "Client " << clients[i].fd <<" Disconnected\n";
					close(clients[i].fd);
					clients[i].fd = -1;

					if (i == max_i)
						while (clients[max_i].fd < 0)
							--max_i;
				}
				else
				{
					send(clients[i].fd, buf, buf_len, 0);
					cout << buf;
				}
			}
		}
	}
	close(so_server);
	return (0);
}
