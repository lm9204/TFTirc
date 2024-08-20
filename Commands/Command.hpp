#ifndef COMMAND_HPP
# define COMMAND_HPP
# include "Server.hpp"

# define RPL_WELCOME				"001"
# define ERR_NOSUCHNICK				"401"
# define ERR_CANNOTSENDTOCHAN		"404"
# define ERR_NOTEXTTOSEND			"412"
# define ERR_NONICKNAMEGIVEN		"431"
# define ERR_ERRONEUSNICKNAME		"432"
# define ERR_NICKNAMEINUSE			"433"
# define ERR_NEEDMOREPARAMS			"461"
# define ERR_ALREADYREGISTERED		"462"

using namespace std;

class Server;
class Client;

class Command {
	public:
		Command();
		Command(const Command& other);
		Command& operator=(const Command& other);
		virtual ~Command();
		virtual void execute(Server& server, Client& client) = 0;
		void setCmdSource(vector<string>& cmdSource);
		const string makeNumericMsg(Server& server, Client& client, const string& num);
		const string makeNumericMsg(Server& server, Client& client, const string& name, const string& num);
	protected:
		vector<string> _cmdSource;
};

#endif