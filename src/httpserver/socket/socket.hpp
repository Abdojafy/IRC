#ifndef SOCKET_HPP
#define SOCKET_HPP


#include "ircserv.hpp"
class Socket {
public:
	Socket(int domain, int type, int protocol, int port, u_long ip);
	~Socket();

	//memberfunctions
	virtual	int connect_network(int fd, struct sockaddr_in adr) = 0;

	//variabl getters
	struct	sockaddr_in get_address();
	int		get_socketfd();
	int		get_connection();

	//variabl setters
	void	set_connection(int connect);
	
private:
	struct sockaddr_in address; 
	int socket_fd;
	int connection;

};

#endif