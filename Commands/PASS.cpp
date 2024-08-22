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
	if (client.getNickName() != "*")
	{
		client.send(makeNumericMsg(server, client, "462"));
		return ;
	}
	if (_cmdSource.size() < 2)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return ;
	}
	if (server.getPassword() != _cmdSource[1])
	{
		client.send(makeNumericMsg(server, client, "464"));
		//break client;
		return ;
	}
	client.send("PASS: AUTHENTICATE");
}
