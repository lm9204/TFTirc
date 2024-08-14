#pragma once
#ifndef CAP_HPP
# define CAP_HPP

# include "Command.hpp"

class CAP: public Command {
	public:
		CAP();
		CAP(const CAP& other);
		CAP& operator=(const CAP& other);
		~CAP();
		virtual void execute();

	private:
		
};

#endif