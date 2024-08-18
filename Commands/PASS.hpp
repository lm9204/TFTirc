#ifndef PASS_HPP
# define PASS_HPP

# include "Command.hpp"

class Pass : public Command {
	public:
		Pass();
		Pass(const Pass& other);
		Pass& operator=(const Pass& other);
		virtual ~Pass();
		virtual void	execute(Server& server, Client& client);
};

#endif