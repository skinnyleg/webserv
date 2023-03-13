#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include "client.hpp"

#define BACKLOG 10
#define BUFFER 1024

class miniserver
{
	public:
	int socket_server;
	short port;
	int opt;
	std::string str_port;
	struct sockaddr_in address;
	int addrlen;

	miniserver();
	miniserver(const miniserver &obj);
	miniserver& operator=(const miniserver& obj);
	~miniserver();
};