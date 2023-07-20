#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "../clients/client.hpp"
#include "../channels/channels.hpp"
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
	typedef std::map<std::string, channels *>				mapChannels;
	typedef std::map<std::string, channels *>::iterator		channelsIter;

	//lclient map t9dar taccidi lih mn aya blassa nta fiha dir Server::clients_map 
	static ClientMap	clients_map;
	//send message t9dar tsendi biha aya 7aja lclient 3tiha lfd o lmsg 7ta hia t9dar t3ayt liha mnin mabghiti dir Server::send_message
	static void			send_message(int fd, std::string message);
	void				create_bind_listen(int port);
	void				accept_new_client();
	struct in_addr		get_clientip(int fd);
	void				client_isregistred(ClientIter client_iter, std::string hostname, int fd);
	//hna kan9ra aya msg jani mn 3and lclient
	void				read_client_data(PollIter it);
	//hna fin kanregistre client jdid
	int					exec_command(int fd);
	void				set_pass_and_port(char **av);
	void				remove_client(int fd);
	
	//JOIN functions
	void			read_command(PollIter it_client);
	void			join(VecStr command, PollIter it_client);
	bool			check_channel_name(std::string name);
	void			join_channels(PollIter it_client, std::string name, std::string pass);

	//KICK functions
	void			kick(VecStr command, PollIter it_client);
	
	
	//INVITE functions
	void			invite(VecStr command, PollIter it_client);
	void			invite_client(ClientIter it_client, channels *my_channel, ClientIter my_client_it);

	
	std::string		set_welcome_msg(std::string hostname, ClientIter client_iter);

	void			check_pass(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command);
	void			check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, int fd);
	void			check_user(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command);
	void			privmsg(ClientIter client_iter, std::string remind, std::string command, int fd);

private:
	//socket hia fd
	int			server_socket;
	int			client_socket;
	int			poll_result;
	int			port;
	int			client_addr_len;
	std::string	password;
	char 		buffer[BUFFERSIZE];
	std::string	client_msg;
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
void	ft_upper(std::string &msg);
void remove_new_line(std::string &msg);

#endif