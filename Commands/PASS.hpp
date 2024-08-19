#ifndef PASS_HPP
# define PASS_HPP

# include "Command.hpp"

class PASS : public Command {
	public:
		PASS();
		PASS(const PASS& other);
		PASS& operator=(const PASS& other);
		virtual ~PASS();
		virtual void	execute(Server& server, Client& client);
};

#endif