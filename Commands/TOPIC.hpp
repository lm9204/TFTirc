#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define ERR_NOSUCHCHANNEL "403"
# define ERR_CHANOPRIVSNEEDED "482"

class TOPIC: public Command {
	public:
		TOPIC();
		TOPIC(const TOPIC& ref);
		~TOPIC();

		virtual void execute(Server& server, Client& client);
};

#endif