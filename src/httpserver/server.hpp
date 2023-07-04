#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"

#define BUFFERSIZE	1024
#define MAXCLIENTS	1000

class Server{
public:
	Server(char **av);
	~Server();

	void	create_bind_listen(int port);
	void	handle_client();

private:
	Client client[MAXCLIENTS];
	int			server_socket;
	int			client_socket;
	int			poll_fd;
	int			port;
	int			adrlen;
	std::string	password;
	struct	sockaddr_in addr_server;
	struct	sockaddr_in addr_client;
	struct	pollfd fds[MAXCLIENTS];
};
int	parce_port(char *str);
bool is_it_digits(std::string str);

#endif