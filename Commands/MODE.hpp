#ifndef MODE_HPP
# define MODE_HPP

# include "Command.hpp"
# include <sstream>

class MODE : public Command
{
	private:
		string	_key;
		int		_flag;
		int		_limit;
		int		_mode;
	public:
		typedef enum
		{
			FLAG_I,
			FLAG_T,
			FLAG_K,
			FLAG_O,
			FLAG_L,
			FLAG_PLUS,
			FLAG_MINUS,
		} MODE_OPTION;

		MODE();
		MODE(const MODE& other);
		MODE& operator=(const MODE& other);
		virtual ~MODE();
		virtual void	execute(Server& server, Client& client);
		void	do_command(Server& server, Client& client, std::string name);
		int		check_client_oper(vector<Client*> cli_list, Client client);
};

#endif