#include "TOPIC.hpp"

#include <chrono>
#include <iostream>

TOPIC::TOPIC() {}
TOPIC::TOPIC(const TOPIC& ref) { *this = ref; }
TOPIC::~TOPIC() {}

void	TOPIC::execute(Server& server, Client& client)
{
	if (!isRegisterClient(server, client))
		return ;
	string msg = ":" + server.getServername() + " ";
	Channel* ch = server.getChannel(_cmdSource[1]);
	if (ch == NULL)
	{
		client.send(makeNumericMsg(server, client, _cmdSource[1], "403"));
		return ;
	}
	if (_cmdSource.size() < 2)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return ;
	}
	if (!ch->checkUserInChannel(client.getNickName()))
	{
		client.send(makeNumericMsg(server, client, ch->getName(), "442"));
		return;
	}
	if ((ch->getMode(ch->TOPIC_OPER_ONLY) && ch->isOper(&client)) || !ch->getMode(ch->TOPIC_OPER_ONLY))
	{
		if (_cmdSource.size() == 2)
		{
			if (ch->getTopic().length() == 0)
				msg += "331 " + client.getNickName() + " " + ch->getName() + " :No topic is set\r\n";
			else
			{
				msg += "332 " + client.getNickName() + " " + ch->getName() + " :" + ch->getTopic() + "\r\n";
				msg += ":" + server.getServername() + " 333 " + client.getNickName() + " " + ch->getName() + " " + ch->getTopicByWho() + " " + to_string(chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count()) + "\r\n";
			}
		}
		else
		{
			if (_cmdSource[2].length() == 1)
				ch->clearTopic();
			else
			{
				ch->setTopic(_cmdSource[2], client.who());
			}

			vector<Client*> users = ch->getUsers();
			for (size_t i = 0; i < users.size(); ++i)
			{
				users[i]->send(":" + client.who() + " TOPIC " + ch->getName() + " :" + ch->getTopic() + "\r\n");
			}
		}
	}
	else
		client.send(makeNumericMsg(server, client, ch->getName(), "482"));
}