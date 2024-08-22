#include "UNKNOWN.hpp"
#include <cctype>
#include <sstream>
#include <iostream>
UNKNOWN::UNKNOWN() {
	
}

UNKNOWN::UNKNOWN(const UNKNOWN& other): Command(other) {

}

UNKNOWN& UNKNOWN::operator=(const UNKNOWN& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

UNKNOWN::~UNKNOWN() {

}

void UNKNOWN::execute(Server& server, Client& client) {
	client.send(makeNumericMsg(server, client, ERR_UNKNOWNCOMMAND));
}
