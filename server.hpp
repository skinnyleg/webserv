#include <iostream>
#include <vector>
#include "miniserver.hpp"

class server
{
	public:
	std::vector<miniserver> servers;
	std::vector<client> clients;
	std::vector<struct pollfd> pfds;
	struct pollfd *pfds_raw;
	int flag_con;
	int poll_count;

	server();
	server(const server &obj);
	server& operator=(const server& obj);
	~server();
	void lunch_servers();
	void monitor();
	void fill();
	void convert();
	void receive(int index);
	void new_connection(int index);
};