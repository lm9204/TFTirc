#include "CommandController.hpp"
#include "CAP.hpp"
#include "NICK.hpp"
#include "USER.hpp"
#include <sstream>


CommandController::CommandController() {
	this->_commands["CAP"] = new CAP();
	this->_commands["NICK"] = new NICK();
	this->_commands["USER"] = new USER();
}

CommandController::~CommandController() {
	while (!this->_commands.empty()) {
		delete this->_commands.begin()->second;
		this->_commands.erase(this->_commands.begin());
	}
}

vector<string> cmdSplit(string cmd) {
	istringstream iss(cmd);
	vector<string> res;
	string tmp;

	while (!iss.eof()) {
		iss >> tmp;
		res.push_back(tmp);
	}
	return res;
}

Command* CommandController::makeCommand(Client& client) {
	string cmd = client.getCommand();
	if (cmd == "no_comand")
		return NULL;
	vector<string> cmds = cmdSplit(cmd);
	for (int i = 0; i < static_cast<int>(cmds.size()); i++)
		std::cout << cmds[i] << std::endl;
	if(_commands[cmds[0]] == NULL)
		return NULL;
	_commands[cmds[0]]->setCmdSource(cmds);
	return this->_commands[cmds[0]];
}