#include"channels.hpp"


// std::map<int, Client> channels::client;

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
		// delete this->client;
}

std::string channels::get_name()
{
	return (name);
}


void	channels::set_name(std::string &new_name)
{
	this->name = new_name;
}

std::string channels::get_topic()
{
	return (topic);
}


void	channels::set_topic(std::string &new_topic)
{
	this->topic = new_topic;
}

std::string& channels::get_mode()
{
	// if (!mode.empty())
	// 	mode = "+" + mode;
	return (mode);
}

void	channels::set_mode(std::string &new_mode)
{
	this->mode = new_mode;
}

void	channels::join_mode(std::string new_mode)
{
	this->mode += new_mode;
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

std::string	channels::get_users()
{
	std::string users;
	std::string oper_name;
	bool is_oper = false;
	ClientIter it = client.begin();
	while (it != client.end())
	{
		for(ClientIter oper = operators.begin(); oper != operators.end();oper++)
		{
			if (it->first == oper->first)
			{
				if (!oper_name.empty())	
				{
					oper_name += " ";
				}
				oper_name += "@";
				oper_name += it->second.get_client_nick();
				is_oper = true;
			}
		}
		if (!is_oper)
		{
			users += it->second.get_client_nick();
			users += " ";
		}
		it++;
		is_oper = false;
	}
	users += oper_name;
	return (users);
}
