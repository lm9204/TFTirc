#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <string>
# include <vector>

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
		Channel(string name, string owner);
		~Channel();
		
		vector<string>	getUsers() const;
		vector<string>	getOper() const;
		string	getName() const;
		string	getTopic() const;
		string	getPassword() const;

		int		isOper(string user) const;

		void	setOper(string user);
		void	removeOper(string user);
		void	setPassword(string password);
		void	removePassword();
		void	join(string user);
		void	leave(string user);
		void	kick(string user);
		void	setTopic(string topic);

		int		getMode(CHANNEL_OPT type) const;
		void	setMode(CHANNEL_OPT type, int value);
		// void	broadcast(string msg);


		string	_getTimestamp() const;
	private:
		string	_name;
		string	_password;
		string	_topic;

		vector<string>	_users;
		vector<string>	_operators;

		/* Channel Options */
		int		_inviteOnly;
		int		_topicOpOnly;
		int		_user_limit;

		int		_exist(vector<string> group, string nick) const;
		int		_find(vector<string> group, string nick) const;

};
std::ostream& operator<<(std::ostream& os, const Channel& ref);

#endif