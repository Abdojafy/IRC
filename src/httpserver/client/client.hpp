#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserv.hpp"

class Client: public Socket{
public:
	//constructors 
	Client(int domain, int type, int protocol, int port, u_long ip);
	~Client();

	//memberfunctions
	int connect_network(int fd, struct sockaddr_in address);
private:
};

#endif