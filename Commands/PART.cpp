#include "PART.hpp"

PART::PART() {}
PART::~PART() {}

void	PART::execute(Server& server, Client& client)
{
	string		ch_name = _cmdSource[1];
	Channel*	ch = server.getChannel(ch_name);

	if (_cmdSource.size() < 2)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return;
	}
	if (ch == NULL)
	{
		client.send(makeNumericMsg(server, client, ch_name, "403"));
		return;
	}
	vector<Client*> users = ch->getUsers();
	for (vector<Client*>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickName() == client.getNickName())
		{
			ch->broadcast(client.who() + " PART " + ch_name + "\r\n");
			return ;
		}
	}
	client.send(makeNumericMsg(server, client, "442"));
}