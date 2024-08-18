#pragma once
#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

# define ERR_NONICKNAMEGIVEN 431
# define ERR_ERRONEUSNICKNAME 432
# define ERR_NICKNAMEINUSE 433

class NICK: public Command {
	public:
		NICK();
		NICK(const NICK& other);
		NICK& operator=(const NICK& other);
		virtual ~NICK();
		virtual void execute(Server& server, Client& parser);
	private:
		string	makeNumericMsg(Server& server, Client& client, int num);
		bool	checkNickName(const string& nickName);
};

#endif