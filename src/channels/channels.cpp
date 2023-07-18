#include"channels.hpp"

channels::channels()
{
	
}

channels::channels(std::string name)
{
	if (name.length() < 200)
		this->name = name;
}

channels::channels(std::string name, std::string password, std::string mode)
{
	if (name.length() < 200)
	{
		this->name = name;
		this->password = password;
		this->mode = mode;
	}
}

channels::~channels()
{
	//delete channels;
}

std::string channels::get_name()
{
	return (name);
}


void	channels::set_name(std::string &new_name)
{
	this->name = new_name;
}

std::string channels::get_mode()
{
	return (mode);
}

void	channels::set_mode(std::string &new_mode)
{
	this->mode = new_mode;
}

std::string channels::get_password()
{
	return (password);
}

void	channels::set_password(std::string &new_password)
{
	this->password = new_password;
}

size_t channels::get_limite()
{
	return (limite);
}

void	channels::set_limite(size_t new_limite)
{
	this->limite = new_limite;
}

bool channels::get_invited()
{
	return (invited);
}

void	channels::set_invited(bool new_invited)
{
	this->invited = new_invited;
}

std::string	channels::get_users()
{
	std::string users;
	ClientIter it = client.begin();
	while (it != client.end())
	{
		if (it != client.begin())
			users += " ";
		users += it->second.get_client_nick();
		it++;
	}
	return (users);
}
