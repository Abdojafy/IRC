#ifndef SOCKET_HPP
#define SOCKET_HPP


#include "ircserv.hpp"
class Socket {
public:
	Socket(int domain, int type, int protocol, int port, u_long ip);
	~Socket();

	virtual int connect_network(int fd, struct sockaddr_in adr) = 0;
private:
	struct sockaddr_in address; 
	int socket_fd;
	int connection;

};

#endif