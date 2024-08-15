#include "NICK.hpp"

NICK::NICK() {
	
}

NICK::NICK(const NICK& other): Command(other) {

}

NICK& NICK::operator=(const NICK& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

NICK::~NICK() {

}

void NICK::execute(Server& server, Client& parser) {
	static_cast<void>(server);
	static_cast<void>(parser);
}