#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../../inc/ircserv.hpp"

class Client{
public:
	Client();
	//constructors 


	//memberfunctions
	int	start_client();
private:
	struct	sockaddr_in address;
	int		client_fd;
	int		addrlen;

};

#endif