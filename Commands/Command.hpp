#pragma once
#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"
# include <iostream>

class Command {
	public:
		Command();
		Command(const Command& other);
		Command& operator=(const Command& other);
		virtual ~Command();
		virtual void execute(Server& server, Parser& parser) = 0;
		void setCmdSource(vector<string>& cmdSource);
		string makeNumericMsg(const string& hostName, const string& number); // Numeric 메시지 생성함수. 아직 미구현

	protected:
		vector<string> _cmdSource;
};

#endif