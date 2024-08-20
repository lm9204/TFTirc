#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"



class PRIVMSG: public Command {
	public:
		PRIVMSG();
		PRIVMSG(const PRIVMSG& other);
		PRIVMSG& operator=(const PRIVMSG& other);
		virtual ~PRIVMSG();
		virtual void execute(Server& server, Client& parser);
	private:
		void fillSendTarget();
		vector<string> sendTarget;
};

#endif