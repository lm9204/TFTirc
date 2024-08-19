#ifndef SERVER_HPP
# define SERVER_HPP
# include <map>
# include <vector>
# include <unistd.h>

# include <sys/socket.h>
# include <sys/event.h>
# include <fcntl.h>
# include <arpa/inet.h>

# include "Client.hpp"
# include "Channel.hpp"
# include "CommandController.hpp"


# define MAX_USER 512
# define HOST "TFTirc"

class CommandController;

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
		int		createChannel(string ch_name, string owner);

		void	deleteChannel(string ch_name); // *
		string	getPassword() const; // *
		string	getHostname() const;
		
		Client*		getClient(int fd);
		Client*		getClient(string nick);
		Channel*	getChannel(string ch_name);


	private:
		//error Client(-1)
		CommandController		_command_controller;
		vector<Channel> _channels;
		vector<Client>	_clients;
		Client	_err_client;
		Channel	_err_channel;
		string	_password;
		string	_name;
		int		_port;
		
		vector<struct kevent>	_change_list;
		struct sockaddr_in		_server_addr;
		struct kevent			_event_list[8];
		int						_socket;


};

void	handle_error(string err);

#endif