#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <vector>
# include <unistd.h>

using namespace std;

# define BUF_SIZE 1024

class Client
{
	public:
		Client(int fd);
		~Client();

		vector<string>	split(string line, char delim);
		string	who() const;		//nick!nick@host.com
		int		recv();
		int		send(const string& msg) const;

		string	getNickName() const;
		string	getHostName() const;
		string	getRealName() const;
		string	getBufName() const;
		int		getSocketFd() const;
		string	getCommand();

		void	setNickName(string nick);
		void	setUserName(string user);
		void	setHostName(string host);
		void	setRealName(string real);

		string flushBuf();
		void setBuf();
	private:
		string	_nick;
		string	_user;
		string	_host;
		string	_real;

		string	_buf;
		int		_fd;
};

#endif