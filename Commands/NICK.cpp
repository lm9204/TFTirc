#include "NICK.hpp"
#include <cctype>
#include <sstream>

NICK::NICK() {
	
}

NICK::NICK(const NICK& other): Command(other) {

}

NICK& NICK::operator=(const NICK& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

NICK::~NICK() {

}

void NICK::execute(Server& server, Client& client) {
	// 파라미터 확인
	if (this->_cmdSource.size() < 2) {
		client.send(makeNumericMsg(server, client, ERR_NONICKNAMEGIVEN));
		return ;
	}
	// 유효 닉네임 확인
	if (!checkNickName(this->_cmdSource[1])) {
		client.send(makeNumericMsg(server, client, ERR_ERRONEUSNICKNAME));
		return ;
	}
	// 중복 닉네임 확인
	Client* findClient = server.getClient(this->_cmdSource[1]);
	if (findClient != NULL) {
		client.send(makeNumericMsg(server, client, ERR_NICKNAMEINUSE));
		return ;
	}
	server.notify(client.getNickName(), ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " " + this->_cmdSource[0] + " " + this->_cmdSource[1] + "\r\n");
	client.send(":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " " + this->_cmdSource[0] + " " + this->_cmdSource[1] + "\r\n");
	if (client.getNickName() == "*" && client.getUserName() != "" && client.getRealName() != "") {
		client.setNickName(this->_cmdSource[1]);
		client.send(makeNumericMsg(server, client, RPL_WELCOME));
	} else 
		client.setNickName(this->_cmdSource[1]);
}

/*
- 모든 영숫자 문자, 대괄호 및 중괄호([]{}), 백슬래시(\), 파이프(|) 문자를 허용
- 첫번째 문자는 숫자를 허용하지 않음
- 명령어에 모호성을 주지 않는한 추가문자 허용 가능
*/
bool	NICK::checkNickName(const string& nickName) {
	if (isdigit(nickName[0]))
		return false;
	string allowChar("[]{}\\|");
	for (string::const_iterator it = nickName.begin() + 1; it != nickName.end(); it++)
		if (!isalnum(*it) && allowChar.find(*it) == string::npos)
			return false;
	return true;
}