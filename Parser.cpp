#include "Parser.hpp"

Parser::Parser(std::string buf, int socket_fd) : _buf(buf), _socket_fd(socket_fd) {}
Parser::~Parser()
{
}
std::string Parser::get_command()
{
    std::string     input(_buf);
    std::string     buffer;
    std::string     line;
    std::string     command;
    std::istringstream   iss(input);
    if (std::getline(iss, buffer))
    {
		buffer = buffer.substr(0, input.find("\r\n"));
        _buf = _buf.substr(input.find("\r\n") + 2, input.size() - 1);
        return (buffer);
    }
    else
        return ("");
}

std::vector<std::string>    Parser::split_command(std::string command)
{
    std::string     input(command);
    std::string     buffer;
    std::istringstream   iss(input);
    std::vector<std::string> ret;
    while (std::getline(iss, buffer))
    {
		buffer = buffer.substr(0, input.find("\r\n"));
        ret.push_back(buffer);
    }
    return (ret);
}
