#pragma once
#ifndef BOT_HPP
# define BOT_HPP

# include "Client.hpp"
# include "Server.hpp"

# define BOT_NAME "TFT_Bot"

using namespace std;

class Server;

class Bot: public Client {
	public:
		Bot(Server* server);
		virtual ~Bot();
		virtual int		send(const string& msg) const;
	private:
		Bot();
		Bot(const Bot& other);
		Bot& operator=(const Bot& other);
		Server* server;
};

#endif
