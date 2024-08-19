#include "USER.hpp"
#include <sstream>


USER::USER() {
	
}

USER::USER(const USER& other): Command(other) {

}

USER& USER::operator=(const USER& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

USER::~USER() {

}


//
//    ERR_NEEDMOREPARAMS (461)
//    ERR_ALREADYREGISTERED (462)

void USER::execute(Server& server, Client& client) {
	// 인자 수 체크
	if (this->_cmdSource.size() < 5) {
		client.send(makeNumericMsg(server, client, ERR_NEEDMOREPARAMS));
		return ;
	}
	if (client.getUserName() != "*" && client.getUserName() != "" && client.getRealName() != "") {
		client.send(makeNumericMsg(server, client, ERR_ALREADYREGISTERED));
		return ;
	}
	client.setUserName(this->_cmdSource[1]);
	client.setRealName(this->_cmdSource[4]);
	if (client.getNickName() != "*")
		client.send(makeNumericMsg(server, client, RPL_WELCOME));
}