#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"
#define bufferSize 1024

class Server{
public:
	Server(char **av);
	~Server();

	void	create_bind_listen(int port);
	void	handle_client();

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