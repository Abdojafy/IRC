#include "ircserv.hpp"
#include "server.hpp"

Server::Server(int domain, int type, int protocol, int port, u_long ip): Socket(domain, type, protocol, port, ip)
{
	set_connection(connect_network(get_socketfd(), get_address()));
	if (get_connection() < 0)
	{
		perror("bind");
		exit(1);
	}
}

Server::~Server()
{

}

int Server::connect_network(int fd, sockaddr_in address)
{
	return (bind(fd, (struct sockaddr *)&address, sizeof(address)));
}
