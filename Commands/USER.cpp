#include "USER.hpp"

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

void USER::execute(Server& server, Client& parser) {
	static_cast<void>(server);
	parser.send(":server 001 <client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]\r\n");
}