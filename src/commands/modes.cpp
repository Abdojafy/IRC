#include"../server/server.hpp"



void	Server::o_mode(bool take, channelsIter my_channel, ClientIter client_it, ClientIter my_client)
{	
	std::string message;
	ClientIter op = my_channel->second->operators.find(client_it->first);
	if (op == my_channel->second->operators.end())
	{
		if (take)
		{
			my_channel->second->operators.insert(std::make_pair(client_it->first, client_it->second));
			//:mine!~t@197.230.30.146 MODE #abdoo +o mine
			message =  ":" + my_client->second.get_client_nick() + "!~" + my_client->second.get_client_username() + "@" + my_client->second.get_clientip() + " MODE " + my_channel->first + " +o " + client_it->second.get_client_nick() + "\r\n" ;
			send_message(my_client->first, message);
			return;
		}
	}
	else
	{
		if (!take && my_channel->second->operators.size() > 1)
		{
			my_channel->second->operators.erase(client_it->first);
			//:mine!~t@197.230.30.146 MODE #abdoo -o mine
			message =  ":" + my_client->second.get_client_nick() + "!~" + my_client->second.get_client_username() + "@" + my_client->second.get_clientip() + " MODE " + my_channel->first + " -o " + client_it->second.get_client_nick() + "\r\n" ;
			send_message(my_client->first, message);
			return;
		}	
	}
}

// void	Server::i_mode(bool take, channelsIter my_channel, ClientIter client_it, ClientIter my_client)
// {

// }

void	Server::l_mode(bool take, channelsIter my_channel, ClientIter my_client, std::string flag)
{
	std::string message;
	size_t lim = std::atoi(flag.c_str());
	//ClientIter my_client = clients_map.find(client_it->first);
	if (my_channel->second->get_mode().find("l") == std::string::npos)
	{
		if (take && (lim > 0 || flag == "0"))
		{
			my_channel->second->join_mode("l");
			my_channel->second->set_limite(lim);
			//:mine!~t@197.230.30.146 MODE #abdoo +l limite
			message =  ":" + my_client->second.get_client_nick() + "!~" + my_client->second.get_client_username() + "@" + my_client->second.get_clientip() + " MODE " + my_channel->first + " +l " + flag + "\r\n" ;
			send_message(my_client->first, message);
			return;
		}
	}
	else
	{
		if (take && (lim > 0 || flag == "0"))
		{
			my_channel->second->set_limite(lim);
			//:mine!~t@197.230.30.146 MODE #abdoo +l limite
			message =  ":" + my_client->second.get_client_nick() + "!~" + my_client->second.get_client_username() + "@" + my_client->second.get_clientip() + " MODE " + my_channel->first + " +l " + flag + "\r\n" ;
			send_message(my_client->first, message);
			return;
		}
		if (!take)
		{
			(my_channel->second->get_mode()).erase(my_channel->second->get_mode().find("l"), 1);
			//:mine!~t@197.230.30.146 MODE #abdoo -l
			message =  ":" + my_client->second.get_client_nick() + "!~" + my_client->second.get_client_username() + "@" + my_client->second.get_clientip() + " MODE " + my_channel->first + " -l" + "\r\n" ;
			send_message(my_client->first, message);
			return;
		}	
	}
}

// void	Server::k_mode(bool take, channelsIter my_channel, ClientIter client_it, ClientIter my_client)
// {

// }

// void	Server::t_mode(bool take, channelsIter my_channel, ClientIter client_it, ClientIter my_client)
// {

// }


void	Server::modes(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.find(it_client->fd);
	ClientIter client_it = clients_map.begin();
	std::string flag;
	std::string channel;
	std::string mode;
	bool	take;
	std::string message;

	if (command.size() < 4 && command.size() > 1)
	{
		channel = *it++;
		mode = *it++;
		if (command.size() == 3)
			flag = *it;
		channelsIter my_channel = listChannels.find(channel);
		if (my_channel == listChannels.end())
		{
			//:punch.wa.us.dal.net 403 hello hel :No such channel
			message = ":" + my_client_it->second.get_clientip() + " 403 " + my_client_it->second.get_client_nick() + " " + channel + " :No such channel\r\n";
			send_message(it_client->fd, message);
			return;
		}
		my_client_it = my_channel->second->operators.find(it_client->fd);
		if (my_client_it == my_channel->second->operators.end())
		{
			//:zinc.libera.chat 482 youssef #general :You're not a channel operator
			message = ":" + my_client_it->second.get_clientip() + " 482 " + my_client_it->second.get_client_nick() + " " + channel + " :You're not a channel operator\r\n";
			send_message(it_client->fd, message);
			return;
		}
		if (mode.size() == 2)
		{
			if(mode[0] == '+')
				take = true; 
			else if(mode[0] == '-')
				take = false; 
			else
			{
				//error
				return;
			}
			mode.erase(0, 1);
			if (mode == "o")
			{
				if (command.size() != 3)
				{
					message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " MODE :Not enough parameters\r\n";
					send_message(it_client->fd, message);
					return;
				}
				while (client_it != clients_map.end())
				{
					if (!strcmp(client_it->second.get_client_nick().c_str(), flag.c_str()))
						break; 
					client_it++;
				}
				if (client_it == clients_map.end())
				{
					//:punch.wa.us.dal.net 403 hello hel :No such nick
					message = ":" + my_client_it->second.get_clientip() + " 401 " + my_client_it->second.get_client_nick() + " " + channel + " :No such nick/channel\r\n";
					send_message(it_client->fd, message);
					return;
				}
				o_mode(take, my_channel, client_it, my_client_it);
			}
			// else if (mode == "i")
			// 	i_mode(take, my_channel, client_it, my_client_it);
			else if (mode == "l")
			{
				if (take && command.size() != 3)
				{
					message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " MODE :Not enough parameters\r\n";
					send_message(it_client->fd, message);
					return;
				}
				l_mode(take, my_channel, my_client_it, flag);
			}
			// else if (mode == "k")
			// 	k_mode(take, my_channel, client_it, my_client_it);
			// else if (mode == "t")
			// 	t_mode(take, my_channel, client_it, my_client_it);
			else
			{
				//error
				return;
			}
		}
	}
	else
	{
		message = ":" + my_client_it->second.get_clientip() + " 461 " + my_client_it->second.get_client_nick() + " MODE :Not enough parameters\r\n";
		send_message(it_client->fd, message);
	}

}
