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

void	PASS::execute(Server& server, Client& client)
{
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
		server.disconnect_client(client.getSocketFd());
		return ;
	}
	//client.send("PASS: AUTHENTICATE"); -> 필요없음
}
