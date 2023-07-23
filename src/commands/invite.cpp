#include "../server/server.hpp"

void	Server::invite_client(ClientIter it_client, channels *my_channel, ClientIter my_client_it)
{
	std::string message;
	
	//client not in channel
	ClientIter target = my_channel->client.find(my_client_it->first);
	if (target == my_channel->client.end())
	{
			message = ":" + my_client_it->second.get_clientip() + " 442 " + my_client_it->second.get_client_nick() + " " + my_channel->get_name() + " :You're not on that channel\r\n";
			send_message(my_client_it->first, message);
			return;
	}

	//invited is already in channel
	target = my_channel->client.find(it_client->first);
	if (target != my_channel->client.end())
	{
			message = ":" + my_client_it->second.get_clientip() + " 443 " + my_client_it->second.get_client_nick() + " " + it_client->second.get_client_nick() + " " + my_channel->get_name() + " :is already on channel\r\n";
			send_message(my_client_it->first, message);
			return;
	}
	
	
	// channel invited only
	if (my_channel->get_mode().find("i") != std::string::npos)
	{
		target = my_channel->operators.find(my_client_it->first);
		if (target == my_channel->client.end())
		{
			message = ":" + my_client_it->second.get_clientip() + " 482 " + my_client_it->second.get_client_nick() + " " + my_channel->get_name() + " :You're not channel operator\r\n";
			send_message(my_client_it->first, message);
			return;
		}

	}
	my_channel->invited.insert(std::make_pair(it_client->first, it_client->second));
	
	message = ":" + my_client_it->second.get_clientip() + " 341 " + my_client_it->second.get_client_nick() + " " + it_client->second.get_client_nick()  + " " + my_channel->get_name() + "\r\n";
	send_message(it_client->first, message);
	return;
}


void	Server::invite(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.find(it_client->fd);
	ClientIter client_it = clients_map.begin();
	std::string nickname;
	std::string channel;
	std::string message;

	if (command.size() == 2)
	{
		nickname = *it++;
		channel = *it;
		channelsIter my_channel = listChannels.find(channel);
		while (client_it != clients_map.end())
		{
			if (!strcmp(client_it->second.get_client_nick().c_str(), nickname.c_str()))
				break; 
			client_it++;
		}
		if (my_channel == listChannels.end() || client_it == clients_map.end())
		{
			message = ":" + my_client_it->second.get_clientip() + " 401 " + my_client_it->second.get_client_nick() + " " + channel + " :No such nick/channel\r\n";
			send_message(it_client->fd, message);
			return;
		}
		invite_client(client_it, my_channel->second, my_client_it);
	}
	else
	{
		message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " INVITE :Not enough parameters\r\n";
		send_message(it_client->fd, message);
	}

}
