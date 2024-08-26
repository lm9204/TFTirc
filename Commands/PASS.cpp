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
	if (client.getVerifyStatus())
		return ;
	if (_cmdSource.size() < 2)
	{
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	if (client.getNickName() != "*")
	{
		client.send(makeNumericMsg(server, client, ERR_ALREADYREGISTERED));
		return ;
	}
	if (server.getPassword() != _cmdSource[1])
	{
		client.send(makeNumericMsg(server, client, ERR_PASSWDMISMATCH));
		server.disconnect_client(client.getSocketFd());
		return ;
	}
	client.verify();
	//client.send("PASS: AUTHENTICATE"); -> 필요없음
}
