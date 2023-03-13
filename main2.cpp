// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <poll.h>
#include <iostream>

#define PORT1 8080
#define BUFFER 1024
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in6 address;
	int opt = 1;
	std::string header;
	int addrlen = sizeof(address);
	char buffer[BUFFER] = { 0 };
	// std::string buffer;
	std::vector<struct pollfd> pfds;
	struct pollfd *pfds_raw;
	// struct pollfd *pfds = malloc(sizeof(pfds) * fd_size)
	// char* hello = "Hello from server";

	memset(buffer, 0, BUFFER);
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfds.push_back(pfd);
	struct pollfd c;

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin6_family = AF_INET6;
	address.sin6_addr = in6addr_any;
	address.sin6_port = htons(PORT1);
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	int flag = 0;
	int i = 0;
	std::ofstream file;
    while (1)
    {
		pfds_raw = pfds.data();
		int poll_count = poll(pfds_raw, pfds.size(), -1);
		std::cout << "poll count == " << poll_count << std::endl;
		for (int i = 0; i < pfds.size(); i++)
		{
			if (pfds_raw[i].revents & POLLIN)
			{
				std::cout << "event happened" << std::endl;
				if (pfds_raw[i].fd == server_fd)
				{
					if ((new_socket
						= accept(server_fd, (struct sockaddr*)&address,
								(socklen_t*)&addrlen))
						< 0) {
						perror("accept");
						exit(EXIT_FAILURE);
					}
					flag = 0;
					c.fd = new_socket;
					c.events = POLLIN;
					pfds.push_back(c);
					// if (!file.is_open())
					// 	file.close();
					std::cout << "new client connected" << std::endl;
				}
				else
				{
					std::cout << "in to read " << std::endl;
					int bytes = recv(pfds_raw[i].fd, buffer, BUFFER, 0);
					if (bytes == 0)
					{
						std::cout << "client disconnected" << std::endl;
						pfds.erase(pfds.begin() + i);
					}
					else if (bytes > 0)
					{
						std::string str(buffer, bytes);
						std::cout << str << std::endl;
						int res;
						int pos = 0;
						while (pos < str.size() && flag < 1)
						{
							res = str.find("\r\n", pos);
							if (res != std::string::npos)
							{
								if (str[res] == '\r' && str[res + 1] == '\n')
								{
									res += 2;
									if (str[res] == '\r' && str[res + 1] == '\n')
									{
										// std::cout << " inside if 1" << std::endl;
										header = str.substr(0, res + 1);
										// std::cout << "extracted header begin" << std::endl;
										// std::cout << header << std::endl;
										// std::cout << "extracted header end" << std::endl;
										flag = 1;
										str.erase(0, res + 2);
										// std::cout << "extracted str begin" << std::endl;
										// std::cout << str << std::endl;
										// std::cout << "extracted str end" << std::endl;
										// exit(1);
										break ;
									}
									else
										pos = res;
								}
							}
							else
								pos += 1;
						}
						if (flag == 1)
						{
							res = header.find("pdf");
							if (res != std::string::npos)
							{
								std::cout <<  "lol1" << std::endl;
								file.open("file.pdf");
							}
							res = header.find("mp4");
							if (res != std::string::npos)
							{
								std::cout <<  "lol2" << std::endl;
								file.open("file.mp4");
							}
							res = header.find("png");
							if (res != std::string::npos)
							{
								std::cout <<  "lol3" << std::endl;
								file.open("file.png");
							}
							res = header.find("text");
							if (res != std::string::npos)
							{

								std::cout <<  "lol4" << std::endl;
								file.open("file");
							}
							flag++;
							std::cout << "flag == " << flag << std::endl;
						}
						// std::cout << str;
						// std::cout.flush();
						std::cout << str << std::endl;
						file << str;
						file.flush();
						// std::cout.flush();
						memset(buffer, 0, BUFFER);
						// std::string request(buffer, bytes);

						// Parse request
						// std::istringstream request_stream(request);
						// std::string method, uri, version;
						// request_stream >> method >> uri >> version;
						// std::cout << request << std::endl;
						// std::cout << "method " << method << " uri " << uri << " ver " << version << std::endl;
						// // Construct response
						// std::string response_body = "Hello, World! how are you doing haitam?\n";
						// std::ostringstream response_stream;
						// response_stream << "HTTP/1.1 200 OK\r\n"
						// 			<< "Content-Type: text/plain\r\n"
						// 			<< "Content-Length: " << response_body.size() << "\r\n"
						// 			<< "\r\n"
						// 			<< response_body;

						// std::string response = response_stream.str();

						// // Send response to client
						// send(pfds_raw[i].fd, response.c_str(), response.size(), 0);
					}
					else if (bytes < 0)
					{
						exit(1);
						// std::cout << "error" << std::endl;
					}
				}
			}
		}
    }
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
