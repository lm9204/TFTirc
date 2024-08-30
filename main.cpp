#include "Server.hpp"

int main(int argc, char* argv[])
{
	if (argc != 3 || atoi(argv[1]) == -1 || strlen(argv[2]) == 0)
		return (1);
	
	Server server(atoi(argv[1]), argv[2]);
	server.run();

	return (0);
}