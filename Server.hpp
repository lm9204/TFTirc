#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <map>
# include <vector>
# include <unistd.h>

# include <sys/socket.h>
# include <sys/event.h>
# include <fcntl.h>
# include <arpa/inet.h>

# include "Client.hpp"
# include "BufferHandler.hpp"

# define MAX_USER 512

using namespace std;
class Server
{
	public:
		Server(int port, string password);
		~Server();

		void	run();
		void	change_events(vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		void	disconnect_client(int client_fd, map<int, string>& clients);

		int		bindClient();
		Client&	getClient(int fd);
		Client&	getClient(string nick);

	private:
		//error Client(-1)
		//kqueue fd
		//kevents->changelist
		//eventlist
		//vector<client*> _clients;
		//channel*	_channels;
		vector<Client>	_clients;
		Client	_err_client;
		string	_password;
		int		_port;
		
		struct sockaddr_in		_server_addr;
		int						_socket;
		vector<struct kevent>	_change_list;
		struct kevent			_event_list[8];



};

void	handle_error(string err);

#endif