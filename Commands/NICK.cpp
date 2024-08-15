#include "NICK.hpp"

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

/*
parser:
	sendMsg(): 파서가 가지고 있는 소켓으로 인자 메시지를 send 해주는 메서드

server:
	addNewClient(): 새 클라이언트를 생성해서 server에 추가하는 메서드.
	findNickName(): 닉네임 확인하기
*/
void NICK::execute(Server& server, Client& parser) {
	static_cast<void>(server);
	static_cast<void>(parser);
	// if (server.findClientByNickName(_cmsSource[1]) == NULL)
	// 	server.addNewClient(_cmdSource[1], _cmdSource[4]);
	// else
	// 	parser.sendMsg("<client> <nick> :Nickname is already in use");
}