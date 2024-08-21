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
		int		_mode; //-> 게터세터 만들기 
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
		//void	set_invite_only(Channel &channel);
		//void	remove_invite_only(Channel &channel);
		//void	set_topic_restriction(Channel &channel);
		//void	remove_topic_restriction(Channel &channel);
		//void	set_channel_key(Channel &channel);
		//void	remove_channel_key(Channel &channel);
		//void	give_operator(Channel &channel, Client& client);
		//void	take_operator(Channel &channel, Client& client);
		//void	set_limit(Channel &channel);
		//void	remove_limit(Channel &channel);
};

#endif