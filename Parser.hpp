#ifndef PARSER_HPP
# define PARSER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <sys/socket.h>

class Parser
{
    public:
        Parser(std::string buf, int socket_fd);
        ~Parser();
        std::string get_command();
        std::vector<std::string>    split_command();
        std::vector<std::string>    test_split_command(std::string& cmd);	// cmd 한줄을 공백 기준으로 나누는 메서드
		void 						sendMsg(const std::string& msg);		// 파서가 가지고있는 fd로 메시지 보내는 메서드
    private:
        std::string _buf;
        int         _socket_fd;
};
#endif