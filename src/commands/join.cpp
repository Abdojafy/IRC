#include "../server/server.hpp"

bool	find_char(std::string str, char c)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

bool	Server::check_channel_name(std::string name)
{
	if (!name.empty() && name[0] == '#' && !find_char(name, ',') 
		&& !find_char(name, '\a') && name.length() > 1 && name.length() < 200)
	{
		return(true);
	}
	return(false);
}

void	Server::join_channels(PollIter it_client, std::string name, std::string pass)
{
	ClientIter it_map_client = clients_map.begin();
	channelsIter it_channel = listChannels.begin();
	Client *my_client;
	channels *my_channel;
	std::string mode;
	std::string message;

	//which client
	while (it_map_client != clients_map.end())
	{
		if (it_map_client->first == it_client->fd)
		{
			my_client = &it_map_client->second;
			break ;
		}
		it_map_client++;
	}
	if (it_map_client == clients_map.end())
	{
		std::cout << "error client not found" << std::endl;
		exit(1);
	}

	//which channel
	while (it_channel != listChannels.end())
	{
		if (it_channel->first == name)
		{
			my_channel = it_channel->second;
			break ;
		}
		it_channel++;
	}

	//add new channel  
	if (it_channel == listChannels.end())
	{
		if (!pass.empty())
			mode += "k";
		my_channel = new channels(name, pass, mode);
		my_channel->operators.insert(std::make_pair(it_map_client->first, *my_client));
	}

	//already joined to the channel
	ClientIter joined = my_channel->client.find(my_client->get_client_socket());
	if (joined != my_channel->client.end())
		return;

	//check password in mode (+k)
	if ((my_channel->get_mode()).find("k") != std::string::npos)
	{
		if (it_channel != listChannels.end())
		{
			if (strcmp(it_channel->second->get_password().c_str(), pass.c_str()))
			{
				message = ":" + my_client->get_clientip() + " 475 " + my_client->get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+k)\r\n";
				send_message(it_client->fd, message);
				return;
			}
		}
	}


	//check limit in mode (+l)
	if ((my_channel->get_mode()).find("l") != std::string::npos)
	{
		if (my_channel->client.size() >= my_channel->get_limite())
		{
			message = ":" + my_client->get_clientip() + " 471 " + my_client->get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+l)\r\n";
			send_message(it_client->fd, message);
			return;
		}
	}

	//check invite-only in mode (+i)
	if ((my_channel->get_mode()).find("i") != std::string::npos)
	{
		it_map_client = my_channel->invited.begin();
		while (it_map_client != my_channel->invited.end())
		{
			if (it_map_client->first == it_client->fd)
				break;
			it_map_client++;
		}
		if (it_map_client == my_channel->invited.end())
		{
			message = ":" + my_client->get_clientip() + " 473 " + my_client->get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+i)\r\n";
			send_message(it_client->fd, message);
			return;
		}

	}

	// // insert a channel successfully
	my_channel->client.insert(std::make_pair(it_client->fd, *my_client));
	listChannels.insert(std::make_pair(name, my_channel));
	for(ClientIter joined = my_channel->client.begin(); joined != my_channel->client.end(); joined++)
	{
		message =  ":" + my_client->get_client_nick() + "!~" + my_client->get_client_username() + "@" + my_client->get_clientip() + " JOIN " + name + "\r\n" ;
		send_message(joined->first, message);
	}
	message.clear();
	if (!my_channel->get_mode().empty())
		message = ":" + my_client->get_clientip() + " MODE " + name + " +" + my_channel->get_mode() + "\r\n";
	if (!my_channel->get_topic().empty())
		message += ":" + my_client->get_clientip() + " 332 " + my_client->get_client_nick() + " " + name + " :" + my_channel->get_topic() + "\r\n";
	message += ":" + my_client->get_clientip() + " 353 " + my_client->get_client_nick() + " = " + name + " :" + my_channel->get_users() + "\r\n";
	message += ":" + my_client->get_clientip() + " 366 " + my_client->get_client_nick() + " " + name + " :End of /NAMES list." + "\r\n";
	send_message(it_client->fd, message);
}

void	Server::join(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.begin();
	std::string pass;
	std::string name;
	std::string message;
	std::istringstream names;
	std::istringstream passwords;
	
	//which client
	while (my_client_it != clients_map.end())
	{
		if (my_client_it->first == it_client->fd)
			break;
		my_client_it++;
	}
	//error if no client (possible ctrl + C)
	if (my_client_it == clients_map.end())
	{
		std::cout << "error no client " << std::endl;
		exit(1);
	}
	//check params and join
	if (command.size() < 3 && !command.empty())
	{
		names.str(*it++);
		if (it != command.end())
			passwords.str(*it);
		while (std::getline(names, name,','))
		{
			std::getline(passwords, pass,',');
			if (check_channel_name(name))
			{
				channelsIter itch = listChannels.begin();
				while (itch != listChannels.end())
				{
					if (itch->first == name)
					{
						join_channels(it_client, name, pass);
						break ;
					}
					itch++;
				} 
				if (itch == listChannels.end())
				{
					join_channels(it_client, name, pass);
				}
			}
			else
			{
				message = ":" + my_client_it->second.get_clientip() + " 403 " + my_client_it->second.get_client_nick() + " " + name + " :No such channel\r\n";
				send_message(it_client->fd, message);
			}
			pass.clear();
		}
	}
	else
	{
		message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " JOIN :Not enough parameters\r\n";
		send_message(it_client->fd, message);
	}
}
