#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include"ircserv.hpp"
#include "client.hpp"

class channels
{
private:
	std::string name;
	std::string mode;
	std::string password;
public:
	typedef std::map<int, Client>::iterator					ClientIter;
	typedef std::map<int, Client>							ClientMap;

	ClientMap client;
	ClientMap banned;
	channels();
	channels(std::string name);
	channels(std::string name, std::string password);
	~channels();
	std::string 	get_name();
	void			set_name(std::string &new_name);
	std::string 	get_mode();
	void			set_mode(std::string &new_mode);
	std::string 	get_password();
	void			set_password(std::string &new_password);
	void			create_channel(std::string name, std::string mode, std::string password);
};

#endif