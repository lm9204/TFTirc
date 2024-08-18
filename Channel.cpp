#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "Channel.hpp"

Channel::Channel(string empty) : _name(empty)
{
	cout << "[INFO][" << _getTimestamp() << "] Error Channel object created.\n";
}

Channel::Channel(string name, string owner) : _name(name), _inviteOnly(0), _topicOpOnly(1), _user_limit(0)
{
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << name << "] Created Successfully by " << owner << ".\n";
	join(owner);
	setOper(owner);
}

Channel::~Channel()
{
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel deleted.\n";
}

vector<string>	Channel::getUsers() const
{
	return this->_users;
}

vector<string>	Channel::getOper() const
{
	return this->_operators;
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
int		Channel::isOper(string user) const
{
	return _exist(_operators, user);
}

/* need oper */
void	Channel::setOper(string user)
{
	if (_exist(_operators, user))
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " is already has permission.\n";
	else
	{
		_operators.push_back(user);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " is assigned an operator.\n";
	}
}

/* need oper */
void	Channel::removeOper(string user)
{
	int idx;
	if ((idx = _find(_operators, user)) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user << "is not operator.\n";
	else
	{
		_operators.erase(_operators.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] The operator has been removed from the " << user << ".\n"; 
	}
}

/* need oper */
void	Channel::setPassword(string password)
{
	this->_password = password;
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Password Successfully changed to " << password << ".\n";
}

/* need oper */
void	Channel::removePassword()
{
	this->_password = "";
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Password Successfully removed.\n";
}

void	Channel::join(string user)
{
	if (_exist(_users, user))
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " is already in the channel.\n";
	else
	{
		_users.push_back(user);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " joined the channel.\n";
	}
}

void	Channel::leave(string user)
{
	int idx;
	if ((idx = _find(_users, user)) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user << "is not in the channel.\n";
	else
	{
		if (isOper(user))
			removeOper(user);
		_users.erase(_users.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " left the channel.\n"; 
	}
}

/* need oper */
void	Channel::kick(string user)
{
	int idx;
	if ((idx = _find(_users, user)) < 0)
		cout << "[ERROR][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " is not in the channel.\n";
	else
	{
		_users.erase(_users.begin() + idx);
		cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] " << user << " kicked from the channel.\n";
	}
}

void	Channel::setTopic(string topic)
{
	this->_topic = topic;
	cout << "[INFO][" << _getTimestamp() << "][Channel: " << _name << "] Channel topic changed to " << topic << ".\n";
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

// 각 클라이언트들에게 send가 불가능
// void	Channel::broadcast(string msg)
// {

// }


/** @brief Get index if a nickname exists in the group.
 * 
 *  @param	group : A vector of strings representing the group of nicknames.
 *  @param	nick : The nickname to search for within the group.
 *  @return The index of the nickname if it exists in the group, otherwise -1.
 **/
int		Channel::_find(vector<string> group, string nick) const
{
	for (size_t i = 0; i < group.size(); ++i)
		if (group[i] == nick)
			return (i);
	return (-1);
}

/** @brief Checks if a nickname exists in the group.
 * 
 *  @param	group : A vector of strings representing the group of nicknames.
 *  @param	nick : The nickname to search for within the group.
 *  @return 1 if it exists in the group, otherwise 0.
 **/
int		Channel::_exist(vector<string> group, string nick) const
{
	for (size_t i = 0; i < group.size(); ++i)
		if (group[i] == nick)
			return (1);
	return (0);
}

std::ostream&	operator<<(std::ostream& os, const Channel& ch)
{
	vector<string> users = ch.getUsers();
	os << "----------------------------------------------------\n";
	os << "[INFO][" << ch._getTimestamp() << "][Channel: " << ch.getName() << "] Summary: \n";
	os << "\t - Channel Mode Settings\n";
	os << "\t\t [EDIT TOPIC PERMISSION] : " << (ch.getMode(ch.TOPIC_OPER_ONLY) ? "Operator" : "User") << "\n";
	os << "\t\t [INVITE ONLY] : " << (ch.getMode(ch.INVITE_ONLY) ? "true" : "false") << "\n";
	os << "\t\t [USER LIMIT] : " << ch.getMode(ch.USER_LIMIT) << "\n";
	os << "\t - Current Users (" << users.size() << "):\n";
	for (size_t i = 0; i < users.size(); ++i)
	{
		os << "\t\t [" << i + 1 << "] Name: " << users[i] << ", Role: " << (ch.isOper(users[i]) == 1 ? "Operator" : "User") << "\n";
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