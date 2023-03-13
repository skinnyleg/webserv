#include <iostream>
#include <vector>
#include <fstream>

class client
{
	public:
	int client_socket;
	std::string buffer;
	std::string header_request;
	std::ofstream file;
	int bytes_read;

	void extractheader(char *buff);
	void openfile();
	void response();
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};