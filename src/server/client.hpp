#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserv.hpp"

class Client{
public:
	Client();
	Client(struct sockaddr_in addr, int fd);
	~Client();

	sockaddr_in get_client_address();

	Client &operator=(const Client &other);
private:
	std::string user;
	std::string	nick;
	std::string pass;
	int			client_socket;
	sockaddr_in address;

};

#endif