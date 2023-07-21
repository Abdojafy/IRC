#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include"ircserv.hpp"
#include "../clients/client.hpp"


class channels
{
private:
	std::string 	name;
	std::string 	mode;
	std::string 	password;
	std::string		topic;
	size_t			limite;
	size_t			classement;
public:
	typedef std::map<int, Client>::iterator					ClientIter;
	typedef std::map<int, Client>							ClientMap;

	ClientMap client;
	ClientMap operators;
	ClientMap invited;
	channels();
	channels(std::string name);
	channels(std::string name, std::string password, std::string mode);
	~channels();
	std::string 	get_name();
	void			set_name(std::string &new_name);
	std::string 	get_topic();
	void			set_topic(std::string &new_topic);
	std::string& 	get_mode();
	void			set_mode(std::string &new_mode);
	void			join_mode(std::string new_mode);
	std::string 	get_password();
	void			set_password(std::string &new_password);
	std::string		get_users();
	size_t			get_limite();
	void			set_limite(size_t limite);
	size_t			get_classement();
	void			set_classement(size_t classement);
};

#endif