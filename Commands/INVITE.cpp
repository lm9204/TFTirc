#include "INVITE.hpp"

INVITE::INVITE() {}
INVITE::~INVITE() {}

void	INVITE::execute(Server& server, Client& client)
{
	if (_cmdSource.size() < 3)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return ;
	}

	string	invitee = _cmdSource[1];
	string	inviter = client.getNickName();
	string	ch_name = _cmdSource[2];

	Channel*	ch = server.getChannel(ch_name);
	if (ch == NULL)
	{
		client.send(makeNumericMsg(server, client, ch_name, "403"));
		return ;
	}

	vector<Client*> users = ch->getUsers();
	bool isInviterOnCh = false, isInviterHasOp = false, isInviteOnly = ch->getMode(ch->INVITE_ONLY);
	for (vector<Client*>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickName() == inviter)
		{
			if ((isInviteOnly && ch->isOper(&client)) || !isInviteOnly)
				isInviterHasOp = true;
			isInviterOnCh = true;
		}
		if ((*it)->getNickName() == invitee)
		{
			client.send(":" + server.getServername() + " 443 " + client.getNickName() + " " + invitee + " " + ch->getName() + " :is already on channel\r\n");
			return;
		}
	}

	if (isInviterOnCh && isInviterHasOp)
	{
		Client* target = server.getClient(invitee);
		if (target != NULL)
			target->send(":" + client.who() + " INVITE " + invitee + " :" + ch_name + "\r\n");
		ch->invite(invitee);
		client.send(":" + server.getServername() + " 341 " + inviter + " " + invitee + " " + ch_name + "\r\n");
		
	}
	else if (isInviterOnCh && !isInviterHasOp)
		client.send(makeNumericMsg(server, client, ch_name, "482"));
	else
		client.send(makeNumericMsg(server, client, ch_name, "442"));
}