#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"
#include "ircserv.hpp"

class Client{
public:
	Client(struct sockaddr_in addr, int fd);
	~Client();
private:
	struct sockaddr_in address;
	int		client_socket;
};

#endif