#include "PASS.hpp"

PASS::PASS(): Command() {}
PASS::PASS(const PASS& other): Command(other) {}
PASS::~PASS() {}

PASS& PASS::operator=(const PASS& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

//clien에 flag 있어야한다. ++ -> 플래그 하나로 관리
//PASS아닐 떄 어떤 프로토콜 보내야 하는지?
//getPASSWord

void	PASS::execute(Server& server, Client& client)
{
	//:dan-!d@localhost QUIT :Quit: Bye for now!
	if (server.flag != 0)
		client.send(":" + server.getServerName() + " 462 " + client.getNickName() + " :You may not reregister\r\n");
	else if (server.getPassword() == _cmdSource[1])
	{
		server.flag = 1;
		client.send("PASS: AUTHENTICATE\r\n");
	}
	else if (sizeof(_cmdSource))
		client.send(":" + server.getServerName() + " 461 * PASS :Not enough parameters\r\n");
	else
		client.send(":" + server.getServerName() + " 464 * :PASSword incorrect\r\n");
}
