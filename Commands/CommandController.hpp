#pragma once
#ifndef COMMANDCONTROLLER_HPP
# define COMMANDCONTROLLER_HPP

# include "Client.hpp"
# include <map>
class Command;
class Client;

class CommandController {
	public:
		CommandController();
		~CommandController();
		Command* makeCommand(Client& parser);
	private:
		std::map<string, Command*> _commands;
};

#endif