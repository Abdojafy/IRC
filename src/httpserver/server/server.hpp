#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"

class Server: Socket{
public:
	Server(int domain, int type, int protocol, int port, u_long ip);
	~Server();

	int connect_network(int fd, struct sockaddr_in adress);

private:

};

#endif