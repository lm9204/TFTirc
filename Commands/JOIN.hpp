#ifndef JOIN_HPP
# define JOIN_HPP

# include "Command.hpp"
# include "Server.hpp"
# include "Client.hpp"

class JOIN: public Command {
	public:
		JOIN();
		JOIN(const JOIN& other);
		JOIN& operator=(const JOIN& other);
		virtual ~JOIN();
		virtual void execute(Server& server, Client& parser);
	private:
		void fillTargetChannel();
		void fillTargetKey();
		void scaleTarget();
		bool checkValidChannelName(const string& name);
		vector<string> targetChannel;
		vector<string> targetKey;
};

#endif