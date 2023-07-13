#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"
#include "channels.hpp"
#include <netinet/in.h>		/* in_addr */


#define BUFFERSIZE	1024
#define MAXCLIENTS	1000

class channels;
class Server{
public:
	Server(char **av);
	~Server();
	typedef std::vector<pollfd>::iterator					PollIter;
	typedef std::vector<pollfd> 							PollFds;
	typedef std::map<int, Client>::iterator					ClientIter;
	typedef std::map<int, Client>							ClientMap;
	typedef std::vector<std::string>						VecStr;
	typedef std::vector<std::string>::iterator				VecIter;
	//Channels
	typedef std::map<std::string, channels>					mapChannels;
	typedef std::map<std::string, channels>::iterator		channelsIter;



	typedef std::vector<pollfd>::iterator			PollIter;
	typedef std::vector<pollfd> 					PollFds;
	typedef std::map<int, Client>::iterator			ClientIter;
	typedef std::map<int, Client>					ClientMap;
	typedef std::vector<std::string>				VecStr;
	typedef std::vector<std::string>::iterator		VecIter;
	
	static void		send_message(int fd, std::string message);
	void			create_bind_listen(int port);
	void			accept_new_client();
	struct in_addr	get_clientip(int fd);
	void			read_client_data(PollIter it);
	int				get_client_info(int fd);
	void			set_pass_and_port(char **av);
	void			check_pass(ClientIter client_iter, std::string remind, std::string hostname, std::string &err_msg, int fd, std::string command);
	void			check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, std::string &err_msg , int fd);
	void			check_user(ClientIter client_iter, std::string remind, std::string hostname, std::string &err_msg, int fd, std::string command);
	//JOIN functions
	void			read_command(PollIter it_client);
	void			join(VecStr command, PollIter it_client);
	bool			check_channel_name(std::string name);
	void			join_channels(PollIter it_client, std::string name, std::string pass);

	std::string		set_welcome_msg(std::string hostname, ClientIter client_iter);

	void			check_pass(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command);
	void			check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, int fd);
	void			check_user(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command);

private:
	int			server_socket;
	int			client_socket;
	int			poll_result;
	int			port;
	int			client_addr_len;
	std::string	password;
	char 		buffer[BUFFERSIZE];
	std::string	client_msg;
	ClientMap	clients_map;
	PollFds 	poll_fds;
	sockaddr_in addr_server;
	sockaddr_in addr_client;

	//authentication variables
	VecStr		nick_names;
	std::string	realname;
	std::string	username;
	std::string	nick;
	std::string	pass;

	//JOIN variables
	mapChannels listChannels;
};

int	parce_port(char *str);
bool is_it_digits(std::string str);
std::vector<std::string> split(std::string str, char c);
std::string trim_spaces(std::string str);
std::vector<std::string> ft_split(char const *s, char c);
std::string to_upper(const char *str);


#endif