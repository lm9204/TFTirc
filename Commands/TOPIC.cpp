#include "TOPIC.hpp"

TOPIC::TOPIC() {}
TOPIC::TOPIC(const TOPIC& ref) { *this = ref; }
TOPIC::~TOPIC() {}

void	TOPIC::execute(Server& server, Client& client)
{
	string msg;
	Channel* ch = server.getChannel(_cmdSource[1]);
	if ((ch->getMode(ch->TOPIC_OPER_ONLY) && ch->isOper(&client)) || !ch->getMode(ch->TOPIC_OPER_ONLY))
	{
		if (_cmdSource.size() == 2)
			msg = ch->getName() + " topic is " + ch->getTopic() + ".\r\n"; 
		else
		{
			if (_cmdSource[2].length() == 1)
			{
				ch->clearTopic();
				msg = ch->getName() + " topic is cleared\r\n.";
			}
			else
			{
				string topic = _cmdSource[2].erase(0);
				ch->setTopic(topic);
				msg = ch->getName() + " topic is changed to " + topic + ".\r\n";
			}
		}
	}
	else
		msg = makeNumericMsg(server, client, "482");
	client.send(msg);
}