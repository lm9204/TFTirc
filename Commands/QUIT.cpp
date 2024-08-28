#include "QUIT.hpp"

QUIT::QUIT() {
	
}

QUIT::QUIT(const QUIT& other): Command(other) {

}

QUIT& QUIT::operator=(const QUIT& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

QUIT::~QUIT() {

}
// 당장 보내는 메시지를 이게 전부인것으로 파악됨
// :yeondcho!~w@118.218.39.69 QUIT :Client Quit
// 
void QUIT::execute(Server& server, Client& client) {
	string command;

	command = this->_cmdSource[0];
	server.notify(client.getNickName(), ":" + client.who() + " " + command + " " + ":Client Quit" + "\r\n");
	client.send(":" + client.who() + " " + command + " " + ":Client Quit" + "\r\n");
	server.disconnect_client(client.getSocketFd());
}
