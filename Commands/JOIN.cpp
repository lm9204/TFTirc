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
	if (!isRegisterClient(server, client))
		return ;
	if (this->_cmdSource.size() < 2) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	fillTargetChannel();
	fillTargetKey();
	scaleTarget();
	vector<string>::iterator channelIt, keyIt;
	for (channelIt = this->targetChannel.begin(), keyIt = this->targetKey.begin(); channelIt != this->targetChannel.end() && keyIt != this->targetKey.end(); channelIt++, keyIt++) {
		string channelName = *channelIt;
		if (!checkValidChannelName(channelName)) {
			client.send(makeNumericMsg(server, client, channelName, ERR_BADCHANMASK));
			continue;
		}
		Channel* channel = server.getChannel(channelName);
		bool newChannel = false;
		// 채널이 존재하지 않는 경우
		if (channel == NULL) {
			server.createChannel(channelName, &client);
			channel = server.getChannel(channelName);
			channel->join(&client);
			channel->setOper(&client);
			newChannel = true;
		}
		// Key가 존재하고 Key가 맞지 않을 경우
		if (channel->getPassword() != "" && *keyIt != channel->getPassword()) {
			client.send(makeNumericMsg(server, client, *channel, ERR_BADCHANNELKEY));
			continue;
		}
		// 유저 제한이 존재하고 제한을 넘겼을 경우
		if (channel->getMode(Channel::USER_LIMIT) != 0 && channel->getMode(Channel::USER_LIMIT) <= static_cast<int>(channel->getUsers().size())) {
			client.send(makeNumericMsg(server, client, *channel, ERR_CHANNELISFULL));
			continue;
		}
		// 초대 전용 모드이고 초대되지 않았을 경우
		if (channel->getMode(Channel::INVITE_ONLY) && !channel->isInvited(client.getNickName())) {
			channel->accept(client.getNickName());
			client.send(makeNumericMsg(server, client, *channel, ERR_INVITEONLYCHAN));
			continue;
		}
		if (!newChannel)
			channel->join(&client);
		channel->broadcast(":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getHostName() + " " + "JOIN" + " " + channelName + "\r\n");
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
	if (this->_cmdSource.size() < 3)
		return ;
	string targets = this->_cmdSource[2];
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
		targetKey.push_back("");
	while (targetChannel.size() < targetKey.size())
		targetKey.pop_back();
}

bool JOIN::checkValidChannelName(const string& name) {
	string invalidChar("\x20\x07\x2C");
	if (name[0] != '#')
		return false;
	for (string::const_iterator it = name.begin(); it != name.end(); it++)
		if (invalidChar.find(*it) != string::npos)
			return false;
	return true;
}