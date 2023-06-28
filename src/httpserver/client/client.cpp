#include "ircserv.hpp"
#include "client.hpp"

Client::Client(int domain, int type, int protocol, int port, u_long ip): Socket(domain, type, protocol, port, ip)
{
	set_connection(connect_network(get_socketfd(), get_address()));
	if(get_connection() < 0)
	{
		perror("connect");
		exit(1);`
	}
}

Client::~Client()
{

}

int Client::connect_network(int fd, sockaddr_in address)
{
	return (connect(fd, (struct sockaddr *)&address, sizeof(address)));
}
