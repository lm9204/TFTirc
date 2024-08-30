#include "USER.hpp"
#include <sstream>


USER::USER() {
	
}

USER::USER(const USER& other): Command(other) {

}

USER& USER::operator=(const USER& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

USER::~USER() {

}

void USER::execute(Server& server, Client& client) {
	if (!isVerifyClient(server, client))
		return ;
	// 인자 수 체크
	if (this->_cmdSource.size() < 5) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	if (client.getUserName() != "*" && client.getUserName() != "" && client.getRealName() != "") {
		client.send(makeNumericMsg(server, client, ERR_ALREADYREGISTERED));
		return ;
	}
	string userName, realName;
	userName = this->_cmdSource[1];
	realName = this->_cmdSource[4];
	if (!checkUserName(userName))
		userName = client.getNickName();
	if (!checkRealName(realName))
		realName = client.getNickName();
	client.setUserName(userName);
	client.setRealName(realName);
	if (client.getNickName() != "*") {
		client.send(makeNumericMsg(server, client, RPL_WELCOME));
		Client* bot = server.getClient(BOT_NAME);
		if (bot != NULL)
			client.send(":" + bot->who() + " " + "PRIVMSG" + " " + client.getNickName() + " " + ":저는 럭키비키니시티봇 입니다~~🍀" + "\r\n");
	}
}

bool	USER::checkUserName(const string& userName) {
	for (string::const_iterator it = userName.begin() + 1; it != userName.end(); it++)
		if (!isalnum(*it))
			return false;
	return true;
}

bool	USER::checkRealName(const string& realName) {
	string allowChar(": ");
	for (string::const_iterator it = realName.begin() + 1; it != realName.end(); it++)
		if (!isalnum(*it) && allowChar.find(*it) == string::npos)
			return false;
	return true;
}