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

#define PORT 8080
#define BUFFER 1024
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in6 address;
	int opt = 1;
	int addrlen = sizeof(address);
	unsigned char buffer[BUFFER] = { 0 };
	// std::string buffer;
	std::vector<struct pollfd> pfds;
	struct pollfd *pfds_raw;
	// struct pollfd *pfds = malloc(sizeof(pfds) * fd_size)
	// char* hello = "Hello from server";

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
	address.sin6_port = htons(PORT);
	memset(buffer, 0, BUFFER);
	// Forcefully attaching socket to the port 8080
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
	std::ofstream file("file");
    while (1)
    {
		pfds_raw = pfds.data();
		int poll_count = poll(pfds_raw, pfds.size(), -1);
		for (int i = 0; i < pfds.size(); i++)
		{
			if (pfds_raw[i].revents & POLLIN)
			{
				if (pfds_raw[i].fd == server_fd)
				{
					if ((new_socket
						= accept(server_fd, (struct sockaddr*)&address,
								(socklen_t*)&addrlen))
						< 0) {
						perror("accept");
						exit(EXIT_FAILURE);
					}
					c.fd = new_socket;
					c.events = POLLIN;
					pfds.push_back(c);
					// std::cout << "new client connected" << std::endl;
				}
				else
				{
					int bytes = recv(pfds_raw[i].fd, buffer, BUFFER, 0);
					if (bytes == 0)
					{
						// std::cout << "client disconnected\n";
						pfds.erase(pfds.begin() + i);
					}
					else
					{
						// std::cout << " size is " <<  bytes << " ";
						buffer[bytes] = '\0';
						// std::cout << buffer;
						file << buffer;
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
				}
			}
		}
    }
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
