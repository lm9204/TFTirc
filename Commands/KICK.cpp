#include "KICK.hpp"
#include <iostream>
#include <sstream>

KICK::KICK()
{

}

KICK::KICK(const KICK& ref)
{
	*this = ref;
}

KICK::~KICK()
{

}

KICK&	KICK::operator=(const KICK& ref)
{
	if (this == &ref)
		return *this;

	// nothing can copy it...
	return *this;
}

void	KICK::execute(Server& server, Client& client)
{
	if (!isRegisterClient(server, client))
		return ;
	string	ch_name = this->_cmdSource[1];
	vector<s_msg> msgs = splitMsg(this->_cmdSource[2], this->_cmdSource[3]);
	
	Channel* ch = server.getChannel(ch_name);
	if (ch == NULL)
	{
		client.send(makeNumericMsg(server, client, ch_name, "403"));
		return;
	}
	if (!ch->isOper(&client))
	{
		client.send(makeNumericMsg(server, client, ch_name, "482"));
		return ;
	}

	for (vector<s_msg>::iterator it = msgs.begin(); it != msgs.end(); ++it)
	{
		if (server.getClient((*it).nick) == NULL)
		{
			client.send(makeNumericMsg(server, client, (*it).nick, "401"));
			continue;
		}
		ch->broadcast(":" + client.who() + " KICK " + ch_name + " " +  (*it).nick + " :" + (*it).reason + "\r\n");
		ch->kick(server.getClient((*it).nick));
	}
}

vector<KICK::s_msg>	KICK::splitMsg(string user, string reason)
{
	stringstream	ss_user(user);
	stringstream	ss_reason(reason);
	vector<s_msg>	result;
	s_msg			item;

	if (user.find(',') != std::string::npos)
	{
		while (getline(ss_user, item.nick, ','))
		{
			item.reason = reason;
			result.push_back(item);
		}
	}
	else
	{
		item.nick = user;
		item.reason = reason;
		result.push_back(item);
	}
	return result;
}

