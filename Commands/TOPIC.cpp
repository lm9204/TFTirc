#include "TOPIC.hpp"

#include <chrono>
#include <iostream>

TOPIC::TOPIC() {}
TOPIC::TOPIC(const TOPIC& ref) { *this = ref; }
TOPIC::~TOPIC() {}

void	TOPIC::execute(Server& server, Client& client)
{
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
				msg += "332 " + client.getNickName() + " " + ch->getName() + " :" + ch->getTopic() + "\r\n";
		}
		else
		{
			if (_cmdSource[2].length() == 1)
			{
				ch->clearTopic();
				msg += "332 " + client.getNickName() + " " + ch->getName() + "\r\n";
			}
			else
			{
				string topic = _cmdSource[2].erase(0, 1);
				ch->setTopic(topic);
				msg += "332 " + client.getNickName() + " " + ch->getName() + " :" + ch->getTopic() + "\r\n";
			}

			vector<Client*> users = ch->getUsers();
			for (size_t i = 0; i < users.size(); ++i)
			{
				users[i]->send(msg);
				users[i]->send(":" + server.getServername() + " 333 " + users[i]->getNickName() + " " + ch->getName() + " " + client.getNickName() + " " + to_string(chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count()) + "\r\n");
			}
		}
	}
	else
		client.send(makeNumericMsg(server, client, ch->getName(), "482"));
}