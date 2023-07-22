#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserv.hpp"

class Client{
public:
	Client();
	Client(struct sockaddr_in addr, int fd);
	~Client();

	sockaddr_in get_client_address();
	void		increment_isvalid(std::string command);
	int			get_isvalid();
	//get_client_fd
	int			get_client_socket();
	//check if client registred or not false = not registred true = registred
	bool 		get_registred();
	//set client (ip : hotname)
	void		set_clientip(std::string ip);
	void 		set_registred();
	void		set_client_user(std::string user);
	void		set_client_data(std::string realname, std::string pass);
	void		set_client_nick(std::string  nick);
	std::string	get_client_nick();
	std::string	get_client_username();
	std::string	get_client_realname();
	//get client (ip : hotname)
	std::string	get_clientip();


	Client &operator=(const Client &other);
private:
	std::string username;
	std::string	realname;
	std::string	nick;
	std::string pass;
	int			client_socket;
	std::string	clientip;
	sockaddr_in address;
	//authentication ustilis
	int			isvalid;
	bool 		registred;
	bool		nickbool;
	bool		userbool;
	bool		passbool;
};

#endif