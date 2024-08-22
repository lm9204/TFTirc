#include "JOIN.hpp"
#include "Channel.hpp"
#include <cctype>
#include <sstream>
#include <iostream>

JOIN::JOIN() {
	
}

JOIN::JOIN(const JOIN& other): Command(other) {

}

JOIN& JOIN::operator=(const JOIN& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

JOIN::~JOIN() {

}

void JOIN::execute(Server& server, Client& client) {
	if (client.getNickName() == "*" || client.getUserName() == "" || client.getRealName() == "") {
		client.send(makeNumericMsg(server, client, ERR_NOTREGISTERED));
		return ;
	}
	if (this->_cmdSource.size() < 2) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	fillTargetChannel();
	fillTargetKey();
	scaleTarget();
	vector<string>::iterator channelIt, keyIt;
	for (channelIt = this->targetChannel.begin(), keyIt = this->targetKey.begin(); channelIt != this->targetChannel.end() && keyIt != this->targetKey.end(); channelIt++, keyIt++) {
		this->_cmdSource[1] = *channelIt;
		if (this->_cmdSource[1][0] != '#') {
			client.send(makeNumericMsg(server, client, this->_cmdSource[1], ERR_BADCHANMASK));
			continue;
		}
		Channel* channel = server.getChannel(this->_cmdSource[1]);
		// 채널이 존재하지 않는 경우
		if (channel == NULL) {
			server.createChannel(this->_cmdSource[1], &client);
			channel = server.getChannel(this->_cmdSource[1]);
		}
		// Key가 존재하고 Key가 맞지 않을 경우
		if (channel->getPassword() != "" && *keyIt != channel->getPassword()) {
			client.send(makeNumericMsg(server, client, *channel, ERR_BADCHANNELKEY));
			continue;
		}
		// 초대 전용 모드이고 초대되지 않았을 경우
		if (channel->getMode(Channel::INVITE_ONLY) && !channel->isInvited()) {
				client.send(makeNumericMsg(server, client, *channel, ERR_INVITEONLYCHAN));
			continue;
		}
		// 유저 제한이 존재하고 제한을 넘겼을 경우
		if (channel->getMode(Channel::USER_LIMIT) != 0 && channel->getMode(Channel::USER_LIMIT) <= static_cast<int>(channel->getUsers().size())) {
			client.send(makeNumericMsg(server, client, *channel, ERR_CHANNELISFULL));
			continue;
		}
		channel->join(&client);
		channel->broadcast(":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostName() + " " + "JOIN" + " " + this->_cmdSource[1] + "\r\n");
		client.send(makeNumericMsg(server, client, *channel, RPL_TOPIC));
		client.send(makeNumericMsg(server, client, *channel, RPL_NAMREPLY));
		client.send(makeNumericMsg(server, client, *channel, RPL_ENDOFNAMES));
	}
	this->targetChannel.clear();
	this->targetKey.clear();
}

void JOIN::fillTargetChannel() {
	string targets = this->_cmdSource[1];
	for (string::iterator it = targets.begin(); it != targets.end(); it++)
		if (*it == ',')
			*it = ' ';
	istringstream iss(targets);
	string tmp;
	while (!iss.eof()) {
		iss >> tmp;
		if (tmp != "")
			this->targetChannel.push_back(tmp);
	}
}

void JOIN::fillTargetKey() {
	string targets = this->_cmdSource[1];
	for (string::iterator it = targets.begin(); it != targets.end(); it++)
		if (*it == ',')
			*it = ' ';
	istringstream iss(targets);
	string tmp;
	while (!iss.eof()) {
		iss >> tmp;
		if (tmp != "")
			this->targetKey.push_back(tmp);
	}
}

void JOIN::scaleTarget() {
	while (targetChannel.size() > targetKey.size())
		targetKey.push_back("x");
	while (targetChannel.size() < targetKey.size())
		targetKey.pop_back();
}