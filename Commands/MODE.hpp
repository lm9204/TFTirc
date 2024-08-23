#ifndef MODE_HPP
# define MODE_HPP

# include "Command.hpp"
# include <sstream>

class MODE : public Command
{
	private:
		string			_key;
		int				_flag;
		long long		_limit;
		int				_mode;
	public:
		typedef enum
		{
			FLAG_I,
			FLAG_T,
			FLAG_K,
			FLAG_O,
			FLAG_L,
		} MODE_OPTION;

		MODE();
		MODE(const MODE& other);
		MODE& operator=(const MODE& other);
		virtual ~MODE();
		virtual void	execute(Server& server, Client& client);
		int		check_cmd(Server& server, Channel	&channel, Client& client);
		void	do_command(Server& server, std::string name);
		int		check_client_oper(vector<Client*> cli_list, Client client);
};

#endif