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
	typedef std::vector<pollfd>::iterator	PollIter;
	typedef std::vector<pollfd> 			PollFds;
	typedef std::map<int, Client>::iterator	ClientIter;
	typedef std::map<int, Client>			ClientMap;

	void	create_bind_listen(int port);
	void	accept_new_client();
	void	read_client_data(PollIter it);
	void	set_pass_and_port(char **av);
	void	read_command(char *buffer);
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
	sockaddr_in addr_server;
	sockaddr_in addr_client;
};
int	parce_port(char *str);
bool is_it_digits(std::string str);
#endif