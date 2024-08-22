#ifndef UNKNOWN_HPP
# define UNKNOWN_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

class UNKNOWN: public Command {
	public:
		UNKNOWN();
		UNKNOWN(const UNKNOWN& other);
		UNKNOWN& operator=(const UNKNOWN& other);
		virtual ~UNKNOWN();
		virtual void execute(Server& server, Client& parser);
};

#endif