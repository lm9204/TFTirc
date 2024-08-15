#ifndef USER_HPP
# define USER_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

class USER: public Command {
	public:
		USER();
		USER(const USER& other);
		USER& operator=(const USER& other);
		virtual ~USER();
		virtual void execute(Server& server, Client& parser);
};

#endif