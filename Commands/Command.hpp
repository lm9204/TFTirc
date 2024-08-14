#pragma once
#ifndef COMMAND_HPP
# define COMMAND_HPP

using namespace std;
#include <iostream>

class Command {
	public:
		Command();
		Command(const Command& other);
		Command& operator=(const Command& other);
		~Command();
		virtual void execute() = 0;

	private:
		
};

#endif