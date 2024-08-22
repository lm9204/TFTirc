#include "Server.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4 || atoi(argv[1]) == -1)
		return (1);
	
	Server server(atoi(argv[1]), argv[2]);
	server.run();

	return (0);
}