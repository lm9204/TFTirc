#pragma once
#ifndef USER_HPP
# define USER_HPP

# include "Command.hpp"

class USER: public Command {
	public:
		USER();
		USER(const USER& other);
		USER& operator=(const USER& other);
		~USER();
		virtual void execute();

	private:
		
};

#endif