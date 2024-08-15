#include "CommandController.hpp"
#include "CAP.hpp"
#include "NICK.hpp"
#include "USER.hpp"

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

Command* CommandController::makeCommand(Client& parser) {
	// string cmd = parser.get_command();
	// vector<string> cmdSplit = parser.test_split_command(cmd);
	// for (int i = 0; i < cmdSplit.size(); i++)
	// 	std::cout << cmdSplit[i] << std::endl;
	// _commands[cmdSplit[0]]->setCmdSource(cmdSplit);
	// return this->_commands[cmdSplit[0]];
	(void)parser;
	return (NULL);
	
}