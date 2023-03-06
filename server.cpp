// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iomanip>
#include <vector>
#include <poll.h>
#include <iostream>

#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in6 address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	int fd_size = 5;
	std::vector<struct pollfd> pfds;
	int fd_count = 1;
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

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	pfds_raw = pfds.data();
    while (1)
    {
		int poll_count = poll(pfds_raw, pfds.size(), -1);
		for (int i = 0; i < fd_count; i++)
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
				}
				else
				{
					int bytes
				}
			}
		}
		
        valread = read(new_socket, buffer, 1024);
		if (valread == 0)
		{
			printf("client disconnected\n");
			break ;
		}
        printf("%s", buffer);
		send(new_socket, "message received\n" , strlen("message recieved\n"), 0);
        // send(new_socket, hello, strlen(hello), 0);
        // printf("Hello message sent\n");

        // closing the connected socket
        // closing the listening socket
        // close(new_socket);
    }
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
