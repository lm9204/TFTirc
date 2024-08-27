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
	string nickName;
	string command;

	if (!isVerifyClient(server, client))
		return ;
	// 파라미터 수 확인
	if (this->_cmdSource.size() < 2) {
		client.send(makeNumericMsg(server, client, ERR_NONICKNAMEGIVEN));
		return ;
	}
	command = this->_cmdSource[0];
	nickName = this->_cmdSource[1];
	if (nickName[0] == ':')
		nickName = nickName.substr(1, nickName.size() - 1);
	// 닉네임이 비어있을 때
	if (nickName.empty()) {
		client.send(makeNumericMsg(server, client, ERR_NONICKNAMEGIVEN));
		return ;
	}
	// 유효 닉네임 확인
	if (!checkNickName(nickName)) {
		client.send(makeNumericMsg(server, client, nickName, ERR_ERRONEUSNICKNAME));
		return ;
	}
	// 중복 닉네임 확인
	Client* findClient = server.getClient(nickName);
	if (findClient != NULL) {
		client.send(makeNumericMsg(server, client, nickName, ERR_NICKNAMEINUSE));
		return ;
	}
	// 이미 인증된 유저일 경우
	if (client.getUserName() != "" && client.getRealName() != "") {
		server.notify(":" + client.getNickName(), client.who() + " " + command + " " + nickName + "\r\n");
		client.send(":" + client.who() + " " + command + " " + nickName + "\r\n");
	}
	// 낙네임이 비어있고, 유저네임과 리얼네임이 적혀있는 경우.
	client.setNickName(nickName);
	if (client.getNickName() == "*" && client.getUserName() != "" && client.getRealName() != "")
		client.send(makeNumericMsg(server, client, RPL_WELCOME));
}

/*
- 모든 영숫자 문자, 대괄호 및 중괄호([]{}), 백슬래시(\), 파이프(|) 문자를 허용
- 첫번째 문자는 숫자를 허용하지 않음
- 명령어에 모호성을 주지 않는한 추가문자 허용 가능
*/
bool	NICK::checkNickName(const string& nickName) {
	if (nickName.size() == 0 || isdigit(nickName[0]))
		return false;
	string allowChar("[]{}\\|");
	for (string::const_iterator it = nickName.begin(); it != nickName.end(); it++)
		if (!isalnum(*it) && allowChar.find(*it) == string::npos)
			return false;
	return true;
}