#include "Server.hpp"

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3 || atoi(argv[1]) == -1)
		return (1);
	
	Server server(atoi(argv[1]), "");
	server.run();

	return (0);
}