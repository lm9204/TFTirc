#include "CommandController.hpp"
#include "CAP.hpp"
#include "NICK.hpp"
#include "USER.hpp"
#include "PRIVMSG.hpp"
#include "JOIN.hpp"
#include "MODE.hpp"
#include "PASS.hpp"
#include "KICK.hpp"
#include "TOPIC.hpp"
#include "INVITE.hpp"
#include "PART.hpp"
#include "QUIT.hpp"
#include "UNKNOWN.hpp"
#include <sstream>
#include <iostream>

CommandController::CommandController() {
	this->_commands["CAP"] = new CAP();
	this->_commands["NICK"] = new NICK();
	this->_commands["USER"] = new USER();
	this->_commands["PRIVMSG"] = new PRIVMSG();
	this->_commands["JOIN"] = new JOIN();
	this->_commands["MODE"] = new MODE();
	this->_commands["PASS"] = new PASS();
	this->_commands["KICK"] = new KICK();
	this->_commands["INVITE"] = new INVITE();
	this->_commands["TOPIC"] = new TOPIC();
	this->_commands["PART"] = new PART();
	this->_commands["QUIT"] = new QUIT();
	this->_commands["UNKNOWN"] = new UNKNOWN();
}

CommandController::~CommandController() {
	while (!this->_commands.empty()) {
		delete this->_commands.begin()->second;
		this->_commands.erase(this->_commands.begin());
	}
}

vector<string> CommandController::cmdSplit(string cmd) {
	vector<string> res;
	istringstream iss;
	string beforeColon;
	string afterColon;
	string tmp;
	size_t colon_pos = 0;
	bool flag = false;

	for (colon_pos = 0; colon_pos < cmd.size(); colon_pos++) {
		if (!flag && cmd[colon_pos] == ':')
			break ;
		if (cmd[colon_pos] == '#')
			flag = true;
		if (cmd[colon_pos] == ' ')
			flag = false;
	}
	if (colon_pos < cmd.size()) {
		beforeColon = cmd.substr(0, colon_pos);
		afterColon = cmd.substr(colon_pos);
		afterColon = afterColon.substr(1);
		iss.str(beforeColon);
	} else
		iss.str(cmd);
	while (!iss.eof()) {
		// getline(iss, tmp, ' ');
		iss >> tmp;
		if (tmp != "")
			res.push_back(tmp);
		tmp = "";
	}
	if (colon_pos != cmd.size())
		res.push_back(afterColon);
	return res;
}

Command* CommandController::makeCommand(Client& client) {
	string cmd = client.getCommand();
	if (cmd == "")
		return NULL;
	vector<string> cmds = cmdSplit(cmd);
	if (cmds.empty())
		return NULL;
	for (int i = 0; i < static_cast<int>(cmds.size()); i++)
		cout << "[" << i << "]: " << cmds[i] << std::endl;
	Command* command = this->_commands[cmds[0]];
	if(command == NULL)
		command = this->_commands["UNKNOWN"];
	command->setCmdSource(cmds);
	return command;
}