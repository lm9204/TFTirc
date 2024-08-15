#include "CAP.hpp"

CAP::CAP() {
	
}

CAP::CAP(const CAP& other): Command(other) {

}

CAP& CAP::operator=(const CAP& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

CAP::~CAP() {

}

/*
parser:
	sendMsg(): 파서가 가지고 있는 소켓으로 인자 메시지를 send 해주는 메서드

server:
	getHostName(): 클라이언트에게 알릴 서버 이름.
*/
void CAP::execute(Server& server, Client& parser) {
	static_cast<void>(server);
	parser.send("CAP * LS :\r\n");
	// parser.sendMsg(":" + server.getHostName() + " CAP * LS :" + "\r\n");
}