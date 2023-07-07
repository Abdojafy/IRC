#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"

#define BUFFERSIZE	1024
#define MAXCLIENTS	1000

#include "client.hpp"
class Server{
public:
	Server(char **av);
	~Server();
	typedef std::vector<struct pollfd>::iterator	PollIter;
	typedef std::vector<struct pollfd> 				PollFds;
	typedef std::map<int, Client>::iterator			ClientIter;
	typedef std::map<int, Client>					ClientMap;

	void	create_bind_listen(int port);
	void	handle_client();
	int		get_clientnumber();
	void	accept_new_client();

private:
	int			server_socket;
	int			client_socket;
	int			poll_result;
	int			port;
	int			client_addr_len;
	std::string	password;
	char 		buffer[BUFFERSIZE];
	ClientMap	clients_map;
	PollFds 	poll_fds;
	struct		sockaddr_in addr_server;
	struct		sockaddr_in addr_client;
	struct		pollfd fds[MAXCLIENTS];
};
int	parce_port(char *str);
bool is_it_digits(std::string str);

#endif