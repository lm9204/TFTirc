#include "PASS.hpp"

Pass::Pass(): Command() {}
Pass::Pass(const Pass& other): Command(other) {}
Pass::~Pass() {}

Pass& Pass::operator=(const Pass& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

//clien에 flag 있어야한다. ++ -> 플래그 하나로 관리
//pass아닐 떄 어떤 프로토콜 보내야 하는지?
//getPassWord

void	Pass::execute(Server& server, Client& client)
{
	//:dan-!d@localhost QUIT :Quit: Bye for now!
	if (server.flag != 0)
		client.send(":" + server.getServerName() + " 462 " + client.getNickName() + " :You may not reregister\r\n");
	else if (server.getPassWord() == _cmdSource[1])
	{
		server.flag = 1;
		client.send("PASS: AUTHENTICATE\r\n");
	}
	else if (sizeof(_cmdSource))
		client.send(":" + server.getServerName() + " 461 * PASS :Not enough parameters\r\n");
	else
		client.send(":" + server.getServerName() + " 464 * :Password incorrect\r\n");
}
