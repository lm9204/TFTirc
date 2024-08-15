#pragma once
#ifndef COMMANDCONTROLLER_HPP
# define COMMANDCONTROLLER_HPP

# include "Server.hpp"
# include "Client.hpp"

class Command;

class CommandController {
	public:
		CommandController();
		~CommandController();
		Command* makeCommand(Client& parser);
	private:
		std::map<string, Command*> _commands;
};

#endif