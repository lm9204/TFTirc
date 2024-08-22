#ifndef PART_HPP
# define PART_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define ERR_NOSUCHCHANNEL "403"

class PART: public Command {
	public:
		PART();
		~PART();

		virtual void execute(Server& server, Client& client);
};

#endif