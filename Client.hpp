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
		Client(int fd, string client_addr);
		Client(const Client& ref);
		Client& operator=(const Client& ref);
		virtual ~Client();

		vector<string>	split(string line, char delim);
		string	who() const;		//nick!nick@host.com
		int		recv();
		virtual int		send(const string& msg) const;

		string	getNickName() const;
		string	getUserName() const;
		string	getHostName() const;
		string	getRealName() const;
		int		getVerifyStatus() const;

		string	getBuf() const;
		int		getSocketFd() const;
		string	getCommand();

		void	setNickName(string nick);
		void	setUserName(string user);
		void	setHostName(string host);
		void	setRealName(string real);
		void	verify();

		void	flushBuf();
	private:
		string	_nick;
		string	_user;
		string	_host;
		string	_real;

		string	_buf;
		int		_fd;
		int		_isVerified;
};

std::ostream& operator<<(std::ostream& os, const Client& ref);
#endif