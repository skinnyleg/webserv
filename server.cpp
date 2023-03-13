#include "server.hpp"
#include <sys/ioctl.h>
server::server()
{
	this->flag_con = 0;
	this->poll_count = 0;
}

server::server(const server &obj)
{
	*this = obj;
}

server& server::operator=(const server& obj)
{
	if (this != &obj)
	{
		this->servers = obj.servers;
		this->clients = obj.clients;
		this->pfds = obj.pfds;
		this->pfds_raw = obj.pfds_raw;
		this->flag_con = obj.flag_con;
		this->poll_count = obj.poll_count;
	}
	return (*this);
}

server::~server()
{

}

void server::fill()
{
	miniserver obj;
	obj.port = 8080;
	obj.str_port = std::to_string(obj.port);
	obj.address.sin_family = AF_INET;
	obj.address.sin_addr.s_addr = INADDR_ANY;
	obj.address.sin_port = htons(obj.port);
	obj.addrlen = sizeof(obj.address);
	servers.push_back(obj);
	obj.port = 8081;
	obj.str_port = std::to_string(obj.port);
	obj.address.sin_family = AF_INET;
	obj.address.sin_addr.s_addr = INADDR_ANY;
	obj.address.sin_port = htons(obj.port);
	obj.addrlen = sizeof(obj.address);
	servers.push_back(obj);
}

void server::lunch_servers()
{
	struct pollfd pfd;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if ((servers[i].socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket failed");
			exit(EXIT_FAILURE);
		}
		pfd.fd = servers[i].socket_server;
		pfd.events = POLLIN;
		pfds.push_back(pfd);
		if (setsockopt(servers[i].socket_server, SOL_SOCKET, SO_REUSEADDR, &(servers[i].opt), sizeof(servers[i].opt)))
		{
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		if (bind(servers[i].socket_server, (struct sockaddr*)&(servers[i].address), servers[i].addrlen) < 0)
		{
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		if (listen(servers[i].socket_server, BACKLOG) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}
	}
}

void server::monitor()
{
 	while (1)
    {
		this->pfds_raw = this->pfds.data();
		this->poll_count = poll(this->pfds_raw, this->pfds.size(), -1);
		for (size_t i = 0; i < this->pfds.size(); i++)
		{
			flag_con = 0;
			if (this->pfds_raw[i].revents & POLLIN)
			{
				for (size_t j = 0; j < servers.size(); j++)
				{
					if (pfds_raw[i].fd == servers[j].socket_server)
					{
						this->new_connection(j);
						flag_con = 1;
						break ;
					}
				}
				if (flag_con == 0)
				{
					for (size_t j = 0; j < clients.size(); j++)
					{
						if (pfds_raw[i].fd == clients[j].client_socket)
							this->receive(j);
					}
				}
			}
		}
    }
}

void server::new_connection(int index)
{
	client obj;
	struct pollfd c;
	struct sockaddr_in address;
	unsigned int addrlen;

	if ((obj.client_socket = accept(servers[index].socket_server, (struct sockaddr*)&(address), (socklen_t*)&(addrlen))) < 0) 
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	c.fd = obj.client_socket;
	c.events = POLLIN;
	pfds.push_back(c);
	clients.push_back(obj);
	std::cout << "new client connected" << std::endl;
}

void server::receive(int index)
{
	char buffer[BUFFER];
	memset(buffer, 0, BUFFER);
	clients[index].bytes_read = recv(clients[index].client_socket, buffer, BUFFER, 0);
	if (clients[index].bytes_read == 0)
	{
		std::cout << "client disconnected" << std::endl;
		pfds.erase(pfds.begin() + index + servers.size());
		clients.erase(clients.begin() + index);
	}
	else if (clients[index].bytes_read > 0)
	{
		clients[index].extractheader(buffer);
		clients[index].openfile();
		clients[index].response();
	}
	else if (clients[index].bytes_read < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
}