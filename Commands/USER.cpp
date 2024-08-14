#include "USER.hpp"

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

/*
parser:
	sendMsg(): 파서가 가지고 있는 소켓으로 인자 메시지를 send 해주는 메서드

server:
	addNewClient(): 새 클라이언트를 생성해서 server에 추가하는 메서드.
	findNickName(): 닉네임 확인하기
*/
void USER::execute(Server& server, Parser& parser) {
	static_cast<void>(server);
	parser.sendMsg(":server 001 <client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\r\n");
	// parser.sendMsg(":" + server.getHostName() + " 001 " + server.getClientById(parser.getSocketFd()));
}