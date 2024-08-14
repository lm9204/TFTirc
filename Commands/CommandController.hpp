#pragma once
#ifndef COMMANDCONTROLLER_HPP
# define COMMANDCONTROLLER_HPP

using namespace std;

# include "Command.hpp"
# include "CAP.hpp"
# include "USER.hpp"
# include "Parser.hpp"
# include <vector>
# include <map>

class CommandController {
	public:
		CommandController();
		~CommandController();
		Command* makeCommand(const string& msg);
	private:
		map<string, Command*> _commands;
};

#endif