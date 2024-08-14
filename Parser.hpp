#ifndef PARSER_HPP
# define PARSER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Parser
{
    public:
        Parser(std::string buf, int socket_fd);
        ~Parser();
        std::string get_command();
        std::vector<std::string>    split_command(std::string command);
    private:
        std::string _buf;
        int         _socket_fd;
};
#endif