#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "Channel.hpp"

Channel::Channel(string empty) : _name(empty)
{
	cout << "[INFO][" << _getTimestamp() << "] Error Channel object created.\n";
}

Channel::Channel(string name, Client* owner) : _name(name), _inviteOnly(0), _topicOpOnly(0), _user_limit(0)
{
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << name << "] Created Successfully by " << owner->getNickName() << ".\n";
	setOper(owner);
}

Channel::Channel(const Channel& ref)
{
	*this = ref;
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << ref._name << "] Created Successfully inside vector.\n";
}

Channel::~Channel()
{
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel deleted.\n";
}

Channel&	Channel::operator=(const Channel& ref)
{
	if (this == &ref)
		return *this;
	
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << ref._name << "] Assignment Operator is called.\n";
	this->_name = ref._name;
	this->_inviteOnly = ref._inviteOnly;
	this->_topicOpOnly = ref._topicOpOnly;
	this->_user_limit = ref._user_limit;
	this->_topic = ref._topic;
	this->_password = ref._password;
	for (size_t i = 0; i < ref._users.size(); ++i)
		this->_users.push_back(ref._users[i]);
	for (size_t i = 0; i < ref._operators.size(); ++i)
		this->_operators.push_back(ref._operators[i]);
	return *this;
}

vector<Client*>	Channel::getUsers() const
{
	return this->_users;
}

vector<Client*>	Channel::getOper() const
{
	return this->_operators;
}

vector<string>	Channel::getInvites() const
{
	return this->_invites;
}

string	Channel::getName() const
{
	return this->_name;
}

string	Channel::getTopic() const
{
	return this->_topic;
}

string	Channel::getPassword() const
{
	return this->_password;
}

/* */
int		Channel::isOper(Client* user) const
{
	return _exist(_operators, user->getNickName());
}

int		Channel::checkUserInChannel(string nick) const
{
	return _exist(_users, nick);
}

/* need oper */
void	Channel::setOper(Client* user)
{
	if (!_exist(_users, user->getNickName()))
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " is not in the channel.\n";
	else if (_exist(_operators, user->getNickName()))
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " is already has permission.\n";
	else
	{
		_operators.push_back(user);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " is assigned an operator.\n";
	}
}

/* need oper */
void	Channel::setPassword(string password)
{
	this->_password = password;
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Password Successfully changed to " << password << ".\n";
}

void	Channel::setTopic(string topic)
{
	this->_topic = topic;
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel topic changed to " << topic << ".\n";
}

void	Channel::clearTopic()
{
	this->_topic = "";
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel topic cleared.\n";
}

/* need oper */
void	Channel::removeOper(Client* user)
{
	int idx;
	if ((idx = _find(_operators, user->getNickName())) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << "is not operator.\n";
	else
	{
		_operators.erase(_operators.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] The operator has been removed from the " << user->getNickName() << ".\n"; 
	}
}

/* need oper */
void	Channel::removePassword()
{
	this->_password = "";
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Password Successfully removed.\n";
}

void	Channel::join(Client* user)
{
	if (_exist(_users, user->getNickName()))
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " is already in the channel.\n";
	else
	{
		_users.push_back(user);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " joined the channel.\n";
	}
}

void	Channel::leave(Client* user)
{
	int idx;
	if ((idx = _find(_users, user->getNickName())) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << "is not in the channel.\n";
	else
	{
		if (isOper(user))
			removeOper(user);
		_users.erase(_users.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " left the channel.\n"; 
	}
}

/* need oper */
void	Channel::kick(Client* user)
{
	int idx;
	if ((idx = _find(_users, user->getNickName())) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " is not in the channel.\n";
	else
	{
		_users.erase(_users.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user->getNickName() << " kicked from the channel.\n";
	}
}

void	Channel::broadcast(string msg)
{
	for (vector<Client*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		(*it)->send(msg);
	}
}

void	Channel::broadcast(string msg, Client* except_client)
{
	for (vector<Client*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (except_client != *it)
			(*it)->send(msg);
	}
}

void	Channel::invite(string nick)
{
	for (size_t i = 0; i < _invites.size(); ++i)
	{
		if (_invites[i] == nick)
			return;
	}
	_invites.push_back(nick);
}

void	Channel::accept(string nick)
{
	for (size_t i = 0; i < _invites.size(); ++i)
	{
		if (_invites[i] == nick)
		{
			_invites.erase(_invites.begin() + i);
			return ;
		}
	}
	cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << nick << " is not invited.\n";
}

int Channel::isInvited(string nick)
{
	for (size_t i = 0; i < _invites.size(); ++i)
	{
		if (_invites[i] == nick)
			return (1);
	}
	return (0);
}

int	Channel::getMode(CHANNEL_OPT type) const
{
	switch (type)
	{
		case INVITE_ONLY:
			return _inviteOnly;
		case TOPIC_OPER_ONLY:
			return _topicOpOnly;
		case USER_LIMIT:
			return _user_limit;
		default:
			return (-1);
	}
}

/* need oper */
void	Channel::setMode(CHANNEL_OPT type, int value)
{
	switch (type)
	{
		case INVITE_ONLY:
			_inviteOnly = value;
			cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel INVITE_ONLY option changed to " << (value ? "true" : "false") << ".\n";
			break;
		case TOPIC_OPER_ONLY:
			_topicOpOnly = value;
			cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel TOPIC_OPER_ONLY option changed to " << (value ? "true" : "false") << ".\n";
			break;
		case USER_LIMIT:
			_user_limit = value;
			cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel USER_LIMIT option changed to " << value << ".\n";
			break;
		default:
			break;
	}
}


/** @brief Get index if a nickname exists in the group.
 * 
 *  @param	group : A vector of strings representing the group of nicknames.
 *  @param	nick : The nickname to search for within the group.
 *  @return The index of the nickname if it exists in the group, otherwise -1.
 **/
int		Channel::_find(vector<Client*> group, string nick) const
{
	for (size_t i = 0; i < group.size(); ++i)
		if (group[i]->getNickName() == nick)
			return (i);
	return (-1);
}

/** @brief Checks if a nickname exists in the group.
 * 
 *  @param	group : A vector of strings representing the group of nicknames.
 *  @param	nick : The nickname to search for within the group.
 *  @return 1 if it exists in the group, otherwise 0.
 **/
int		Channel::_exist(vector<Client*> group, string nick) const
{
	for (size_t i = 0; i < group.size(); ++i)
		if (group[i]->getNickName() == nick)
			return (1);
	return (0);
}

std::ostream&	operator<<(std::ostream& os, const Channel& ch)
{
	vector<Client*> users = ch.getUsers();
	vector<string> invites = ch.getInvites();
	os << "----------------------------------------------------\n";
	os << "[INFO][" << ch._getTimestamp() << "][Channel: " << ch.getName() << "] Summary: \n";
	os << "\t - Channel Mode Settings\n";
	os << "\t\t [EDIT TOPIC PERMISSION] : " << (ch.getMode(ch.TOPIC_OPER_ONLY) ? "Operator" : "User") << "\n";
	os << "\t\t [INVITE ONLY] : " << (ch.getMode(ch.INVITE_ONLY) ? "true" : "false") << "\n";
	os << "\t\t [USER LIMIT] : " << ch.getMode(ch.USER_LIMIT) << "\n";
	os << "\t - Current Users (" << users.size() << "):\n";
	for (size_t i = 0; i < users.size(); ++i)
	{
		os << "\t\t [" << i + 1 << "] Name: " << users[i]->getNickName() << ", Role: " << (ch.isOper(users[i]) == 1 ? "Operator" : "User") << "\n";
	}
	os << "\t - Current Invites (" << invites.size() << "):\n";
	for (size_t i = 0; i < invites.size(); ++i)
	{
		os << "\t\t [" << i + 1 << "] Name: " << invites[i] << "\n";
	}
	os << "----------------------------------------------------\n";
	return os;
}

string	Channel::_getTimestamp() const
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(now);
	
	// 스트림에 시간을 포맷팅하여 출력
	stringstream	ss;
	ss << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S");
	
	return ss.str();
}