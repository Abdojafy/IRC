#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"
#define bufferSize 1024

class Server{
public:
	Server(char **av);
	~Server();


private:
	int			server_socket;
	int			client_socket;
	int			port;
	std::string	password;
	struct	sockaddr_in addr_server;
	struct	sockaddr_in addr_client;
	int		adrlen;
};
int	parce_port(char *str);
bool is_it_digits(std::string str);

#endif