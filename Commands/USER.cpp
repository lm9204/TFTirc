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


//
//    ERR_NEEDMOREPARAMS (461)
//    ERR_ALREADYREGISTERED (462)

void USER::execute(Server& server, Client& client) {
	// 인자 수 체크
	if (this->_cmdSource.size() < 5) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	// 이미 등록된 유저
	// if (!client.getUserName().empty()) {
	// 	client.send(makeNumericMsg(server, client, ERR_ALREADYREGISTERED));
	// 	return ;
	// }
	client.setUserName(this->_cmdSource[1]);
	client.setRealName(this->_cmdSource[4]);
	if (client.getNickName() != "*")
		client.send(makeNumericMsg(server, client, RPL_WELCOME));
}

string USER::makeNumericMsg(Server& server, Client& client, const char *num) {
	string res = "";

	static_cast<void>(server);
	// res += ":" + server.getHostName() + " ";
	res += string(":") + "server" + " " + num + " ";
	if (string(num) == string(RPL_WELCOME)) {
		// <client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\r\n
		// res += client.getNickName() + " :Welcome to the " + server.getHostName() + " NetWork, " + client.getNickName() + "!" + client.getUserName() + "@" + server.getHostName() + "\r\n";
		res += client.getNickName() + " :Welcome to the " + "server" + " NetWork, " + client.getNickName() + "!" + "username" + "@" + "server" + "\r\n";
	} else if (string(num) == string(ERR_NEEDMOREPARAMS)) {
		res += client.getNickName() + " " + this->_cmdSource[0] + " " + ":Not enough parameters" + "\r\n";
	} else if (string(num) == string(ERR_ALREADYREGISTERED)) {
		res += client.getNickName() + " " + ":You may not reregister" + "\r\n";
	} else if (string(num) == string(ERR_NONICKNAMEGIVEN)) {
		res += client.getNickName() + " " + ":No nickname given" + "\r\n";
	}
	return res;
}