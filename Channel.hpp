#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <string>
# include <vector>
# include "Client.hpp"

using namespace std;


class Channel
{
	public:
		typedef enum
		{
			INVITE_ONLY,
			TOPIC_OPER_ONLY,
			USER_LIMIT,
		} CHANNEL_OPT;

		Channel(string empty);
		Channel(string name, Client* owner);
		~Channel();
		
		vector<Client*>	getUsers() const;
		vector<Client*>	getOper() const;
		string	getName() const;
		string	getTopic() const;
		string	getPassword() const;

		int		isOper(Client* user) const;

		void	setOper(Client* user);
		void	removeOper(Client* user);
		void	setPassword(string password);
		void	removePassword();
		void	join(Client* user);
		void	leave(Client* user);
		void	kick(Client* user);
		void	setTopic(string topic);

		int		getMode(CHANNEL_OPT type) const;
		void	setMode(CHANNEL_OPT type, int value);
		// void	broadcast(string msg);


		string	_getTimestamp() const;
	private:
		string	_name;
		string	_password;
		string	_topic;

		vector<Client*>	_users;
		vector<Client*>	_operators;

		/* Channel Options */
		int		_inviteOnly;
		int		_topicOpOnly;
		int		_user_limit;

		int		_exist(vector<Client*> group, string nick) const;
		int		_find(vector<Client*> group, string nick) const;

};
std::ostream& operator<<(std::ostream& os, const Channel& ref);

#endif