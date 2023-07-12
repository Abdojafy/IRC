#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserv.hpp"

class Client{
public:
	Client();
	Client(struct sockaddr_in addr, int fd);
	~Client();

	sockaddr_in get_client_address();
	void		increment_isvalid();
	int			get_isvalid();
	bool 		get_registred();
	void		set_clientip(std::string ip);
	void 		set_registred();
	void		set_client_data(std::string username, std::string realname, std::string pass, std::string nick);
	void		set_client_nick(std::string  nick);
	std::string	get_client_nick();


	Client &operator=(const Client &other);
private:
	std::string username;
	std::string	realname;
	std::string	nick;
	std::string pass;
	int			isvalid;
	int			client_socket;
	bool 		registred;
	std::string	clientip;
	sockaddr_in address;

};

#endif