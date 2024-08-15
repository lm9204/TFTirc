#include "Command.hpp"

Command::Command() {
	
}

Command::Command(const Command& other): _cmdSource(other._cmdSource) {
	
}

Command& Command::operator=(const Command& other) {
	if (this == &other)
		return *this;
	this->_cmdSource = other._cmdSource;
	return *this;
}

Command::~Command() {

}

void Command::setCmdSource(vector<string>& cmdSource) {
	this->_cmdSource = cmdSource;
}