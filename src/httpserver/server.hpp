#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../inc/ircserv.hpp"
#define bufferSize 1024

class Server{
public:
	Server(char **av);
	~Server();


private:
	int		server_socket;
	int		client_socket;
	int		port;
	// int		password;
	struct	sockaddr_in addr_server;
	struct	sockaddr_in addr_client;
	int		adrlen;
};
int	parce_port(char *str);
bool is_it_digits(std::string str);

#endif