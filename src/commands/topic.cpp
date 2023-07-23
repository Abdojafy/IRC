#include "../server/server.hpp"

void	Server::topic(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.find(it_client->fd);
	std::string my_topic;
	std::string new_topic;
	std::string channel;
	std::string message;

	if (command.size() < 3 && !command.empty())
	{
		channel = *it++;
		channelsIter my_channel = listChannels.find(channel);
		if (my_channel == listChannels.end())
		{
			message = ":" + my_client_it->second.get_clientip() + " 442 " + my_client_it->second.get_client_nick() + " " + channel + " :You're not on that channel\r\n";
			send_message(it_client->fd, message);
			return;
		}
		if (it != command.end())
		{
			new_topic = *it;
			ClientIter client_operator = my_channel->second->operators.find(it_client->fd);
			if (client_operator == my_channel->second->operators.end() && my_channel->second->get_mode().find("t") != std::string::npos)
			{
				message = ":" + my_client_it->second.get_clientip() + " 482 " + my_client_it->second.get_client_nick() + " " + channel + " :You're not a channel operator\r\n";
				send_message(it_client->fd, message);
				return;
			}
			while (new_topic[0] == ':')
				new_topic.erase(0, 1);
			my_channel->second->set_topic(new_topic);
			for(ClientIter joined = my_channel->second->client.begin(); joined != my_channel->second->client.end(); joined++)
			{
				message =  ":" + my_client_it->second.get_client_nick() + "!~" + my_client_it->second.get_client_username() + "@" + my_client_it->second.get_clientip() + " TOPIC " + my_channel->first + " :" + new_topic +"\r\n" ;
				send_message(joined->first, message);
			}
		}
		else
		{
			my_topic = my_channel->second->get_topic();
			if (my_topic.empty())
			{
				message = ":" + my_client_it->second.get_clientip() + " 331 " + my_client_it->second.get_client_nick() + " " + channel + " :No topic is set\r\n";
				send_message(it_client->fd, message);
				return;
			}
			else
			{
				message = ":" + my_client_it->second.get_clientip() + " 332 " + my_client_it->second.get_client_nick() + " " + channel + " :" + my_topic + "\r\n";
				send_message(it_client->fd, message);
				return;
			}
		}
	}
	else
	{
		message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " TOPIC :Not enough parameters\r\n";
		send_message(it_client->fd, message);
	}
}
