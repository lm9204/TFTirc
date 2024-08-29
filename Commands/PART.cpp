#include "PART.hpp"

#include <iostream>
#include <sstream>

PART::PART() {}
PART::~PART() {}

void	PART::execute(Server& server, Client& client)
{
	if (!isRegisterClient(server, client))
		return ;
	if (_cmdSource.size() < 2)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return;
	}
	vector<string>	chList = splitMsg(_cmdSource[1]);
	for (size_t i = 0; i < chList.size(); ++i)
	{
		string		ch_name = chList[i];
		Channel*	ch = server.getChannel(ch_name);
		
		if (ch == NULL)
		{
			client.send(makeNumericMsg(server, client, ch_name, "403"));
		}
		else if (ch->checkUserInChannel(client.getNickName()))
		{
			if (_cmdSource.size() == 3)
				ch->broadcast(":" + client.who() + " PART " + ch_name + " " + _cmdSource[2] + "\r\n");
			else
				ch->broadcast(":" + client.who() + " PART " + ch_name + "\r\n");
			ch->leave(&client);
			if (ch->getUsers().empty())
				server.deleteChannel(ch->getName());
		}
		else
			client.send(makeNumericMsg(server, client, "442"));
	}
}

vector<string>	PART::splitMsg(string channel)
{
	stringstream	ss_user(channel);
	vector<string>	result;
	string tmp;

	if (channel.find(',') != std::string::npos)
	{
		while (getline(ss_user, tmp, ','))
			result.push_back(tmp);
	}
	else
		result.push_back(channel);
	return result;
}