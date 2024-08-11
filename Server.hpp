#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
	public:
		Server(int port, int password);
		~Server();
		void	run();


	private:
		//kqueue fd
		//kevents->changelist
		//eventlist
		//client* _clients;
		//channel*	_channels;
		int _port;
		int _password;

};

#endif