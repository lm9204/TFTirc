#ifndef INVITE_HPP
# define INVITE_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define ERR_NOSUCHCHANNEL "403"

class INVITE: public Command {
	public:
		INVITE();
		~INVITE();

		virtual void execute(Server& server, Client& client);
};

#endif