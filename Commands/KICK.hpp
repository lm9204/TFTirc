#ifndef KICK_HPP
# define KICK_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define ERR_NOSUCHCHANNEL "403"
# define ERR_NOSUCHNICK "401"

class KICK: public Command {
	public:
		struct s_msg
		{
			string	nick;
			string	reason;
		};

		KICK();
		KICK(const KICK& ref);
		KICK& operator=(const KICK& ref);
		~KICK();

		virtual void execute(Server& server, Client& client);
		vector<string> getMsg(const vector<string>& command);
		vector<s_msg> splitMsg(string user, string reason);
};

#endif