#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../inc/ircserv.hpp"

class Server{
public:
	Server();
	~Server();


private:
	int		server_fd;
	struct	sockaddr_in address;
	int		acpt;
	int		adrlen;
	int		readresult;
};

#endif