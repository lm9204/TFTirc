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
    
	if (_buf.find("\r\n") == std::string::npos)
		return ("no_comand");
	std::getline(iss, buffer);
	buffer = buffer.substr(0, input.find("\r\n"));
    _buf = _buf.substr(input.find("\r\n") + 2, input.size() - 1);
    return (buffer);
}

std::vector<std::string>    Parser::split_command()
{
    std::string     input(_buf);
    std::string     buffer;
    std::istringstream   iss(input);
    std::vector<std::string> ret;

    while (std::getline(iss, buffer))
    {
		if (_buf.find("\r\n") == std::string::npos)
			break;
		buffer = buffer.substr(0, input.find("\r\n"));
        ret.push_back(buffer);
		_buf = _buf.substr(input.find("\r\n") + 2, input.size() - 1);
    }
    return (ret);
}

std::vector<std::string>    Parser::test_split_command(std::string& cmd) {
    std::istringstream   iss(cmd.substr(0, cmd.size() - 2));
	std::string			buffer;
    std::vector<std::string> res;

    while (!iss.eof())
    {
		iss >> buffer;
		res.push_back(buffer);
    }
    return (res);
}

void Parser::sendMsg(const std::string& msg) {
	send(this->_socket_fd, msg.c_str(), msg.size(), 0);
}