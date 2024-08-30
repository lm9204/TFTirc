#include "Bot.hpp"
#include <sstream>

Bot::Bot(Server* server): Client(-1, string(HOST) + string(BOT_NAME)), server(server) {
	this->setNickName(BOT_NAME);
	this->setUserName(BOT_NAME);
	this->setRealName(BOT_NAME);
}

Bot::~Bot() {

}

int		Bot::send(const string& msg) const {
	istringstream iss(msg);
	string source, command, client, mssage, targetNickname;

	iss >> source >> command >> client;
	mssage = iss.str().substr(iss.tellg());
	source[source.find('!')] = ' ';
	iss.str(source.substr(1, source.size() - 1));
	iss >> targetNickname;

	mssage = mssage.substr(0, mssage.size() - 2);
	mssage += "? 완전 럭키비키잖아~~🍀";
	Client* targetClient = this->server->getClient(targetNickname);
	if (targetClient != NULL) {
		return targetClient->send(":" + this->who() + " " + command + " " + targetNickname + " " + mssage + "\r\n");
	}
	return -1;
}