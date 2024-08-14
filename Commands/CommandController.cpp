#include "CommandController.hpp"

CommandController::CommandController() {
	this->_commands["CAP"] = new CAP();
	this->_commands["USER"] = new USER();
}

CommandController::~CommandController() {
	while (!this->_commands.empty()) {
		delete this->_commands.begin()->second;
		this->_commands.erase(this->_commands.begin());
	}
}

Command* CommandController::makeCommand(const string& msg) {
	return this->_commands[msg];
}