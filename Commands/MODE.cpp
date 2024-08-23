#include "MODE.hpp"
#include <iostream>

MODE::MODE(): Command() {}
MODE::MODE(const MODE& other): Command(other) {}
MODE::~MODE() {}

MODE& MODE::operator=(const MODE& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

void	MODE::do_command(Server& server, std::string name)
{
	if (_mode == FLAG_I)
		server.getChannel(name)->setMode(Channel::INVITE_ONLY, _flag);
	else if (_mode == FLAG_T)
		server.getChannel(name)->setMode(Channel::TOPIC_OPER_ONLY, _flag);
	else if (_mode == FLAG_O)
	{
		if (_flag == true)
			server.getChannel(name)->setOper(server.getClient(_key));
		else
			server.getChannel(name)->removeOper(server.getClient(_key));
	}
	else if (_mode == FLAG_K)
	{
		if (_flag == true)
			server.getChannel(name)->setPassword(_key);
		else
			server.getChannel(name)->removePassword();
	}
	else if (_mode == FLAG_L)
	{
		if (_flag == true)
			server.getChannel(name)->setMode(Channel::USER_LIMIT, _limit);
		else
			server.getChannel(name)->setMode(Channel::USER_LIMIT, 0);
	}
}

int		MODE::check_client_oper(vector<Client*> cli_list, Client client)
{
	int len = cli_list.size();
	for (int idx = 0; idx < len; idx++)
	{
		if (cli_list[idx]->getNickName() == client.getNickName())
			return (1);
	}
	return (0);
}

int	MODE::check_cmd(Server& server, Channel	&channel, Client& client)
{
	Client *temp;

	if (_mode == FLAG_I)
	{
		if (_flag == true && channel.getMode(Channel::INVITE_ONLY) == true)
			return (false);
		if (_flag == false && channel.getMode(Channel::INVITE_ONLY) == false)
			return (false);
	}
	if (_mode == FLAG_T)
	{
		if (_flag == true && channel.getMode(Channel::TOPIC_OPER_ONLY) == true)
			return (false);
		if (_flag == false && channel.getMode(Channel::TOPIC_OPER_ONLY) == false)
			return (false);
	}
	if (_mode == FLAG_L)
	{
		if (_limit < 1 || _limit > 2147483647)
			return (false);
		if (_flag == false && channel.getMode(Channel::USER_LIMIT) == -1)
			return (false);
	}
	if (_mode == FLAG_K)
	{
		if (_flag == false && channel.getPassword() == "")
			return (false);
	}
	if (_mode == FLAG_O)
	{
		temp = server.getClient(_key);
		if (temp == NULL)
		{
			client.send(makeNumericMsg(server, client, "401"));
			return (false);
		}
		if (_flag == true && channel.isOper(temp) == true)
			return (false);
		if (_flag == false && channel.isOper(temp) == false)
			return (false);
	}
	return (true);
}

void	MODE::execute(Server& server, Client& client)
{
	string	name;
	string	option;
	string	key;
	string	respond = "";
	string	respond_arg = "";
	std::stringstream ss;
	Channel	*channel;
	int		cmd_idx = 3;
	char	opt;

	if (_cmdSource[1][0] != '#')
	{
		client.send(":" + client.getHostName() + " 221 " + _cmdSource[1] + " " + _cmdSource[2] + "\r\n");
		return ;
	}
	name = _cmdSource[1];
	option = _cmdSource[2];
	_flag = -1;
	channel = server.getChannel(name);
	if (channel == NULL)
	{
		client.send(makeNumericMsg(server, client, "403", name));
		return ;
	}
	if (_cmdSource.size() < 3)
	{
		client.send(makeNumericMsg(server, client, "461"));
		return;
	}
	if (check_client_oper(channel->getOper(), client) != 1)
	{
		client.send(makeNumericMsg(server, client, channel->getName(), "482"));
		return;
	}
	for (int op_idx = 0; op_idx < static_cast<int>(option.size()); op_idx++)
	{
		opt = option[op_idx];
		if (opt == '+')
		{
			_flag = true;
			if (respond != "" && (respond[respond.size() - 1] == '+' || respond[respond.size() - 1] == '-'))
				respond = respond.substr(0, respond.size() - 1);
			respond += opt;
			continue;
		}
		else if (opt == '-')
		{
			_flag = false;
			if (respond != "" && (respond[respond.size() - 1] == '+' || respond[respond.size() - 1] == '-'))
				respond = respond.substr(0, respond.size() - 1);
			respond += opt;
			continue;
		}
		if (opt == 'i')
		{
			_mode = FLAG_I;
		}
		else if (opt == 't')
			_mode = FLAG_T;
		else if (opt == 'l')
		{
			_mode = FLAG_L;
			if (_flag == true)
			{
				if (cmd_idx < static_cast<int>(_cmdSource.size()))
				{
					if (_cmdSource[cmd_idx].size() > 10)
						_limit = 0;
					else
					{
						ss << _cmdSource[cmd_idx];
						ss >> _limit;
					}
				}
				else
					continue;
				cmd_idx++;
			}
		}
		else if (opt == 'k')
		{
			_mode = FLAG_K;
			if (_flag == true)
			{
				if (cmd_idx < static_cast<int>(_cmdSource.size()))
					_key = _cmdSource[cmd_idx];
				else
					continue;
				cmd_idx++;
			}
		}
		else if (opt == 'o')
		{
			_mode = FLAG_O;
			if (_flag == true)
			{
				if (cmd_idx < static_cast<int>(_cmdSource.size()))
					_key = _cmdSource[cmd_idx];
				else
					continue;
				cmd_idx++;
			}
		}
		else
		{
			name = opt;
			client.send(makeNumericMsg(server, client, name, "472"));
			break;
		}
		if (_flag != -1 && check_cmd(server, *channel, client) == true)
		{
			respond += opt;
			do_command(server, name);
			if ((_mode == FLAG_K && _flag == true) ||(_mode == FLAG_L && _flag == true))
			{
				if (respond_arg != "")
					respond_arg += " ";
				respond_arg += _cmdSource[cmd_idx - 1];
			}
		}
	}
	if (respond.size() > 1)
	{
		if (respond_arg != "")
			channel->broadcast(string(":") + client.getNickName() + "!~" +client.getUserName() + "@" + client.getHostName() + " MODE " + channel->getName() + " " + respond + " " + respond_arg + "\r\n");
		else
			channel->broadcast(string(":") + client.getNickName() + "!~" +client.getUserName() + "@" + client.getHostName() + " MODE " + channel->getName() + " " + respond + "\r\n");
	}
}