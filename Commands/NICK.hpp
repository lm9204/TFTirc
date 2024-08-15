#pragma once
#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"

class NICK: public Command {
	public:
		NICK();
		NICK(const NICK& other);
		NICK& operator=(const NICK& other);
		virtual ~NICK();
		virtual void execute(Server& server, Parser& parser);
};

#endif