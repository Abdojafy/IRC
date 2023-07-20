#include "../server/server.hpp"


/*
:youssef!~o@197.230.30.146 TOPIC #ran :hello
*/


void	Server::topic(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.find(it_client->fd);
	std::string my_topic;
	std::string new_topic;
	std::string channel;
	std::string message;

	//if (command.size() > 1)
	if (command.size() < 3 && !command.empty())
	{
		channel = *it++;
		channelsIter my_channel = listChannels.find(channel);
		if (my_channel == listChannels.end())
		{
			//:zinc.libera.chat 442 youssef #rr :You're not on that channel
			message = ":" + my_client_it->second.get_clientip() + " 442 " + my_client_it->second.get_client_nick() + " " + channel + " :You're not on that channel\r\n";
			send_message(it_client->fd, message);
			return;
		}
		if (it != command.end())
		{
			new_topic = *it;
			ClientIter client_operator = my_channel->second->operators.find(it_client->fd);
			if (client_operator == my_channel->second->operators.end())
			{
				//:zinc.libera.chat 482 youssef #general :You're not a channel operator
				message = ":" + my_client_it->second.get_clientip() + " 482 " + my_client_it->second.get_client_nick() + " " + channel + " :You're not a channel operator\r\n";
				send_message(it_client->fd, message);
				return;
			}
			while (new_topic[0] == ':')
				new_topic.erase(0, 1);
			my_channel->second->set_topic(new_topic);
			for(ClientIter joined = my_channel->second->client.begin(); joined != my_channel->second->client.end(); joined++)
			{
				//mine!~r@197.230.30.146 TOPIC #meee :hello
				message =  ":" + my_client_it->second.get_client_nick() + "!~" + my_client_it->second.get_client_username() + "@" + my_client_it->second.get_clientip() + " TOPIC " + my_channel->first + "\r\n" ;
				send_message(joined->first, message);
			}
		}
		else
		{
			my_topic = my_channel->second->get_topic();
			if (my_topic.empty())
			{
				//:zinc.libera.chat 331 youssef #general :No topic is set
				message = ":" + my_client_it->second.get_clientip() + " 331 " + my_client_it->second.get_client_nick() + " " + channel + " :No topic is set\r\n";
				send_message(it_client->fd, message);
				return;
			}
			else
			{
				//:zinc.libera.chat 332 youssef #ran :hello
				message = ":" + my_client_it->second.get_clientip() + " 332 " + my_client_it->second.get_client_nick() + " " + channel + " :" + my_topic + "\r\n";
				send_message(it_client->fd, message);
				return;
			}
		}
	}
	else
	{
		//:zinc.libera.chat 461 youssef TOPIC :Not enough parameters
		message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " TOPIC :Not enough parameters\r\n";
		send_message(it_client->fd, message);
	}
}
