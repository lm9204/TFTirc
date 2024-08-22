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
		if (_flag == FLAG_PLUS)
			server.getChannel(name)->setOper(server.getClient(_key));
		else
			server.getChannel(name)->removeOper(server.getClient(_key));
	}
	else if (_mode == FLAG_K)
	{
		if (_flag == FLAG_PLUS)
			server.getChannel(name)->setPassword(_key);
		else
			server.getChannel(name)->removePassword();
	}
	else if (_mode == FLAG_L)
	{
		if (_flag == FLAG_PLUS)
			server.getChannel(name)->setMode(Channel::USER_LIMIT, _limit);
		else
			server.getChannel(name)->setMode(Channel::USER_LIMIT, -1);
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
		if (_flag == FLAG_PLUS && channel.getMode(Channel::INVITE_ONLY) == FLAG_PLUS)
			return (false);
		if (_flag == FLAG_MINUS && channel.getMode(Channel::INVITE_ONLY) == FLAG_MINUS)
			return (false);
	}
	if (_mode == FLAG_T)
	{
		if (_flag == FLAG_PLUS && channel.getMode(Channel::TOPIC_OPER_ONLY) == FLAG_PLUS)
			return (false);
		if (_flag == FLAG_MINUS && channel.getMode(Channel::TOPIC_OPER_ONLY) == FLAG_MINUS)
			return (false);
	}
	if (_mode == FLAG_L)
	{
		if (_flag == FLAG_MINUS && channel.getMode(Channel::USER_LIMIT) == -1)
			return (false);
	}
	if (_mode == FLAG_K)
	{
		if (_flag == FLAG_MINUS && channel.getPassword() == "") //key 없을 때 기본값 뭐지
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
		if (_flag == FLAG_PLUS && channel.isOper(temp) == true)
			return (false);
		if (_flag == FLAG_MINUS && channel.isOper(temp) == false)
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
	if (check_client_oper(channel->getOper(), client)!= 1)
	{
		client.send(makeNumericMsg(server, client, channel->getName(), "482"));
		return;
	}
	for (int op_idx = 0; op_idx < static_cast<int>(option.size()); op_idx++)
	{
		opt = option[op_idx];
		if (opt == '+')
		{
			_flag = FLAG_PLUS;
			if (respond != "" && (respond[respond.size() - 1] == '+' || respond[respond.size() - 1] == '-'))
				respond = respond.substr(0, respond.size() - 1);
			respond += opt;
			continue;
		}
		else if (opt == '-')
		{
			_flag = FLAG_MINUS;
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
			if (_flag == FLAG_PLUS)
			{
				if (cmd_idx < static_cast<int>(_cmdSource.size()))
				{
					ss << _cmdSource[cmd_idx];
					ss >> _limit;
				}
				else
					continue;
				cmd_idx++;
			}
		}
		else if (opt == 'k')
		{
			_mode = FLAG_K;
			if (_flag == FLAG_PLUS)
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
			if (_flag == FLAG_PLUS)
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
			if ((_mode == FLAG_K && _flag == FLAG_PLUS) ||(_mode == FLAG_L && _flag == FLAG_PLUS))
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
			client.send(string(":") + client.getNickName() + "!~" +client.getUserName() + "@" + client.getHostName() + " MODE " + channel->getName() + " " + respond + " " + respond_arg + "\r\n");
		else
			client.send(string(":") + client.getNickName() + "!~" +client.getUserName() + "@" + client.getHostName() + " MODE " + channel->getName() + " " + respond + "\r\n");
	}
}
	//:<server_name> MODE <channel/user> <mode> <params>
	//어떤 채널에서 명령어를 실행했는지 찾아봐야함.-> 클라이언트 명령어 보고 채널 탐색하기.
	/*
	MODE -> 명령어
	#123 -> 채널이름
	-iiioi+oo -> 모드 옵션
	*/
	// 이런 형식으로 들어오게 된다.
