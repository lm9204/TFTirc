#include "PRIVMSG.hpp"
#include <cctype>
#include <sstream>

PRIVMSG::PRIVMSG() {
	
}

PRIVMSG::PRIVMSG(const PRIVMSG& other): Command(other) {

}

PRIVMSG& PRIVMSG::operator=(const PRIVMSG& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

PRIVMSG::~PRIVMSG() {

}

void PRIVMSG::execute(Server& server, Client& client) {
	if (!isRegisterClient(server, client))
		return ;
	if (this->_cmdSource.size() < 3) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	fillSendTarget();
	for (vector<string>::iterator it = this->sendTarget.begin(); it != this->sendTarget.end(); it++) {
		this->_cmdSource[1] = *it;
		if (this->_cmdSource[1][0] != '#') {
			Client* targetClient = server.getClient(this->_cmdSource[1]);
			if (targetClient == NULL) {
				client.send(makeNumericMsg(server, client, this->_cmdSource[1], ERR_NOSUCHNICK));
			} else {
				targetClient->send(":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostName() + " " + this->_cmdSource[0] + " " + this->_cmdSource[1] + " " + this->_cmdSource[2] + "\r\n");
			}
		} else if (this->_cmdSource[1][0] == '#') {
			Channel* targetChannel = server.getChannel(this->_cmdSource[1]);
			if (targetChannel == NULL) {
				client.send(makeNumericMsg(server, client, this->_cmdSource[1], ERR_CANNOTSENDTOCHAN));
			} else {
				targetChannel->broadcast(":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostName() + " " + this->_cmdSource[0] + " " + this->_cmdSource[1] + " " + this->_cmdSource[2] + "\r\n", &client);
			}
		}
	}
	this->sendTarget.clear();
}

void PRIVMSG::fillSendTarget() {
	string targets = this->_cmdSource[1];
	for (string::iterator it = targets.begin(); it != targets.end(); it++)
		if (*it == ',')
			*it = ' ';
	istringstream iss(targets);
	string tmp;
	while (!iss.eof()) {
		iss >> tmp;
		if (tmp != "")
			this->sendTarget.push_back(tmp);
	}
}