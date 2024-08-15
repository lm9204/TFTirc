#pragma once
#ifndef CAP_HPP
# define CAP_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

class CAP: public Command {
	public:
		CAP();
		CAP(const CAP& other);
		CAP& operator=(const CAP& other);
		virtual ~CAP();
		virtual void execute(Server& server, Client& parser);
};

#endif