#include "Command.hpp"

Command::Command() {
	
}

Command::Command(const Command& other): _cmdSource(other._cmdSource) {
	
}

Command& Command::operator=(const Command& other) {
	if (this == &other)
		return *this;
	this->_cmdSource = other._cmdSource;
	return *this;
}

Command::~Command() {

}

void Command::setCmdSource(vector<string>& cmdSource) {
	this->_cmdSource = cmdSource;
}

const string Command::makeNumericMsg(Server& server, Client& client, const string& num) {
	string res = "";

	res += string(":") + server.getServername() + " " + num + " ";
	if (num == RPL_WELCOME) {
		// res += client.getNickName() + " :Welcome to the " + server.getHostName() + " NetWork, " + client.getNickName() + "!" + client.getUserName() + "@" + server.getHostName() + "\r\n";
		res += client.getNickName() + " :Welcome to the " + "server" + " NetWork, " + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName();
	} else if (num == ERR_NOTEXTTOSEND) {
		res += client.getNickName() + " " + ":No text to send";
	} else if (num == ERR_NONICKNAMEGIVEN) {
		res += client.getNickName() + " " + ":No nickname given";
	} else if (num == ERR_ERRONEUSNICKNAME) {
		res += client.getNickName() + " " + this->_cmdSource[1] + " " + ":Erroneus nickname" + "\r\n";
	} else if (num == ERR_NICKNAMEINUSE) {
		res += client.getNickName() + " " + this->_cmdSource[1] + " " + ":Nickname is already in use" + "\r\n";
	} else if (num == ERR_NEEDMOREPARAMS) {
		res += client.getNickName() + " " + this->_cmdSource[0] + " " + ":Not enough parameters";
	} else if (num == ERR_ALREADYREGISTERED) {
		res += client.getNickName() + " " + ":You may not reregister";
	} else if (num == ERR_NOTREGISTERED) {
		res += client.getNickName() + " " + ":You have not registered";
	} else if (num == ERR_PASSWDMISMATCH) {
		res += client.getNickName() + " " + ":Password incorrect";
	} else if (num == ERR_UNKNOWNCOMMAND) {
		res += client.getNickName() + " " + this->_cmdSource[0] + " " + ":Unknown command";
	} else {
		return "";
	}
	// <crlf>
	res += "\r\n";
	return res;
}

const string Command::makeNumericMsg(Server& server, Client& client, const string& name, const string& num) {
	string res = "";

	res += string(":") + server.getServername() + " " + num + " ";
	if (num == ERR_NOSUCHNICK) {
		res += client.getNickName() + " " + name + " " + ":No such nick/channel";
	} else if (num == ERR_CANNOTSENDTOCHAN) {
		res += client.getNickName() + " " + name + " " + ":Cannot send to channel";
	} else if (num == ERR_NOSUCHCHANNEL) {
		res += client.getNickName() + " " + name + " " + ":No such channel";
	} else if (num == ERR_CHANOPRIVSNEEDED) {
		res += client.getNickName() + " " + name + " " + ":You're not channel operator";
	} else if (num == ERR_UNKNOWNMODE) {
		res += client.getNickName() + " " + name + " " + ":is unknown mode char to me";
	} else if (num == ERR_BADCHANMASK) {
		res += name + " " + ":Bad Channel Mask";
	} else {
		return "";
	}
	// <crlf>
	res += "\r\n";
	return res;
}

const string Command::makeNumericMsg(Server& server, Client& client, Channel& channel, const string& num) {
	string res = "";

	res += string(":") + server.getServername() + " " + num + " ";
	if (num == ERR_BADCHANNELKEY) {
		res += client.getNickName() + " " + channel.getName() + " " + ":Cannot join channel (+k)";
	} else if (num == ERR_CHANNELISFULL) {
		res += client.getNickName() + " " + channel.getName() + " " + ":Cannot join channel (+l)";
	} else if (num == ERR_INVITEONLYCHAN) {
		res += client.getNickName() + " " + channel.getName() + " " + ":Cannot join channel (+i)";
	} else if (num == RPL_TOPIC) {
		res += client.getNickName() + " " + channel.getName() + " " + ":" + channel.getTopic();
	} else if (num == RPL_NAMREPLY) {
		vector<Client*> clients = channel.getUsers();
		res += client.getNickName() + " " + "=" + " " + channel.getName() + " " + ":";
		for (vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
			if (channel.isOper(*it)) {
				res += "@" + (*it)->getNickName();
			} else {
				res += (*it)->getNickName();
			}
			if (it + 1 != clients.end())
				res += " ";
		}
	} else if (num == RPL_ENDOFNAMES) {
		res += client.getNickName() + " " + channel.getName() + " " + ":End of /NAMES list";
	} else {
		return "";
	}
	// <crlf>
	res += "\r\n";
	return res;

}

int Command::checkNotRegisterClient(Server& server, Client& client) {
	if (client.getNickName() == "*" || client.getUserName() == "" || client.getRealName() == "") {
		client.send(makeNumericMsg(server, client, ERR_NOTREGISTERED));
		return 1;
	}
	return 0;
}
