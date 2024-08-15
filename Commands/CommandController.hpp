#pragma once
#ifndef COMMANDCONTROLLER_HPP
# define COMMANDCONTROLLER_HPP

# include "Command.hpp"
# include "CAP.hpp"
# include "USER.hpp"
# include "NICK.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include <vector>
# include <map>

class CommandController {
	public:
		CommandController();
		~CommandController();
		Command* makeCommand(Parser& parser);
	private:
		std::map<string, Command*> _commands;
};

#endif