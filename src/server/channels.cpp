#include"channels.hpp"

void	channels::create_channel(std::string name, std::string mode, std::string password)
{
	channels *new_channel = new channels(name, mode);
	if (!password.empty())
		new_channel->set_password(password);
	
}

channels::channels(std::string name)
{
	if (name.length() < 200)
		this->name = name;
}

channels::channels(std::string name, std::string mode)
{
	if (name.length() < 200)
	{
		this->name = name;
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

