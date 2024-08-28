#ifndef QUIT_HPP
# define QUIT_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

class QUIT: public Command {
	public:
		QUIT();
		QUIT(const QUIT& other);
		QUIT& operator=(const QUIT& other);
		virtual ~QUIT();
		virtual void execute(Server& server, Client& parser);
};

#endif