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

const string Command::makeNumericMsg(Server& server, Client& client, const string& num) {
	string res = "";

	static_cast<void>(server);
	// res += ":" + server.getHostName() + " ";
	// ':<source> <num> '
	res += string(":") + "server" + " " + num + " ";
	if (num == RPL_WELCOME) {
		// res += client.getNickName() + " :Welcome to the " + server.getHostName() + " NetWork, " + client.getNickName() + "!" + client.getUserName() + "@" + server.getHostName() + "\r\n";
		res += client.getNickName() + " :Welcome to the " + "server" + " NetWork, " + client.getNickName() + "!" + client.getUserName() + "@" + "server";
	} else if (num == ERR_NONICKNAMEGIVEN) {
		res += client.getNickName() + " " + ":No nickname given";
	} else if (num == ERR_ERRONEUSNICKNAME) {
		res += client.getNickName() + " " + this->_cmdSource[1] + " " + ":Erroneus nickname" + "\r\n";
	} else if (num == ERR_NICKNAMEINUSE) {
		res += client.getNickName() + " " + this->_cmdSource[1] + " " + ":Nickname is already in use" + "\r\n";
	} else if (num == ERR_ALREADYREGISTERED) {
		res += client.getNickName() + " " + ":You may not reregister";
	} else if (num == ERR_NEEDMOREPARAMS) {
		res += client.getNickName() + " " + this->_cmdSource[0] + " " + ":Not enough parameters";
	} else {
		return "";
	}
	// <crlf>
	res += "\r\n";
	return res;
}