#include "CAP.hpp"

CAP::CAP() {
	
}

CAP::CAP(const CAP& other): Command(other) {

}

CAP& CAP::operator=(const CAP& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

CAP::~CAP() {

}

void CAP::execute(Server& server, Client& client) {
	static_cast<void>(server);
	for (vector<string>::iterator it = this->_cmdSource.begin(); it != this->_cmdSource.end(); it++)
		if (*it == "END")
			return ;
	client.send("CAP * LS :\r\n");
}