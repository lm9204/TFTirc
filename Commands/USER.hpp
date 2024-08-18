#ifndef USER_HPP
# define USER_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

# define RPL_WELCOME "001"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_ALREADYREGISTERED "462"

class USER: public Command {
	public:
		USER();
		USER(const USER& other);
		USER& operator=(const USER& other);
		virtual ~USER();
		virtual void execute(Server& server, Client& parser);
	private:
		string makeNumericMsg(Server& server, Client& client, const char *num);
};

#endif