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
# include "Parser.hpp"

#define BUF_SIZE 1024
#define MAX_USER 512

using namespace std;
class Server
{
	public:
		Server(int port, string password);
		~Server();

		void	run();
		void	change_events(vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		void	disconnect_client(int client_fd, map<int, string>& clients);

		int		apply_client(int client_socket);

	private:
		//kqueue fd
		//kevents->changelist
		//eventlist
		//vector<client*> _clients;
		//channel*	_channels;
		vector<Client *> _clients;
		string	_password;
		int		_port;
		int		_socket;

};

void	handle_error(string err);

#endif