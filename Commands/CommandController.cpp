#include "CommandController.hpp"
#include "CAP.hpp"
#include "NICK.hpp"
#include "USER.hpp"
#include "PRIVMSG.hpp"
#include "JOIN.hpp"
#include "UNKNOWN.hpp"
#include <sstream>
#include <iostream>

CommandController::CommandController() {
	this->_commands["CAP"] = new CAP();
	this->_commands["NICK"] = new NICK();
	this->_commands["USER"] = new USER();
	this->_commands["PRIVMSG"] = new PRIVMSG();
	this->_commands["JOIN"] = new JOIN();
	this->_commands["UNKNOWN"] = new UNKNOWN();
}

CommandController::~CommandController() {
	while (!this->_commands.empty()) {
		delete this->_commands.begin()->second;
		this->_commands.erase(this->_commands.begin());
	}
}

vector<string> CommandController::cmdSplit(string cmd) {
	string beforeColon;
	string afterColon;
	istringstream iss;
	vector<string> res;
	string tmp;

	try {
		beforeColon = cmd.substr(0, cmd.find(":", 0));
		afterColon = cmd.substr(cmd.find(":", 0));
		iss.str(beforeColon);
	} catch (exception& e) {
		iss.str(cmd);
	}
	while (!iss.eof()) {
		iss >> tmp;
		if (tmp != "")
			res.push_back(tmp);
		tmp = "";
	}
	if (afterColon != "")
		res.push_back(afterColon);
	return res;
}

Command* CommandController::makeCommand(Client& client) {
	string cmd = client.getCommand();
	if (cmd == "no_comand")
		return NULL;
	vector<string> cmds = cmdSplit(cmd);
	// for (int i = 0; i < static_cast<int>(cmds.size()); i++)
	// 	cout << "[" << i << "]: " << cmds[i] << std::endl;
	Command* command = this->_commands[cmds[0]];
	if(command == NULL)
		command = this->_commands["UNKNOWN"];
	command->setCmdSource(cmds);
	return command;
}