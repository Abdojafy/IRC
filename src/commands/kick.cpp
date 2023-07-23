#include"../server/server.hpp"

void	Server::ctlc_kick(ClientIter cit, channelsIter channels_iter){

	if (channels_iter->second->client.size() >= 1)
		channels_iter->second->client.erase(cit);
	if (channels_iter->second->operators.size() == 1 && channels_iter->second->client.size() >= 1)
		channels_iter->second->operators.insert(std::make_pair(channels_iter->second->client.begin()->first, channels_iter->second->client.begin()->second));
	ClientIter op_it = channels_iter->second->operators.find(cit->first);
	if(op_it != channels_iter->second->operators.end())
		channels_iter->second->operators.erase(op_it);
	ClientIter inv_it = channels_iter->second->invited.find(cit->first);
	if(inv_it != channels_iter->second->invited.end())
		channels_iter->second->invited.erase(inv_it);
	
}

void	Server::kick(ClientIter client_iter, std::string remind, std::string command, int fd, std::string hostname)
{
	std::stringstream	ss;
	std::string			channelname;
	std::string			clientname;
	VecStr				vec;
	ClientIter			cit;
	channels			channel;
	bool				is_nickname = false;
	bool				is_admin = false;

	ss << remind;
	std::getline(ss, channelname, ' ');
	vec = split(remind, ' ');
	std::getline(ss, clientname, ' ');
	channelsIter channels_iter = listChannels.find(channelname);
	if (vec.size() < 2)
		send_message(fd, ":" + hostname + " 461 " + client_iter->second.get_client_nick() + " " + command + " :Not enough parameters\r\n");
	else if (channels_iter != listChannels.end()){
		for (ClientIter it = channels_iter->second->operators.begin(); it != channels_iter->second->operators.end(); it++){
			if (it->second.get_client_nick() == client_iter->second.get_client_nick()){
				is_admin = true;
				break;
			}
		}
		for (cit = channels_iter->second->client.begin(); cit != channels_iter->second->client.end(); cit++){
			if (cit->second.get_client_nick() == clientname){
				is_nickname = true;
				break;
			}
		}
		if (is_admin || cit->first == fd){
			if (is_nickname){
				send_message(cit->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + " :" + clientname + "\r\n");
				if (channels_iter->second->client.size() == 1)
				{
					channels_iter->second->client.erase(cit);
				}
				else if (cit->first == fd && channels_iter->second->operators.size() == 1)
				{
					channels_iter = listChannels.find(channelname);
					channels_iter->second->client.erase(cit);
					ClientIter oper_it = channels_iter->second->operators.find(cit->first);
					if (oper_it != channels_iter->second->operators.end())
					{
						channels_iter->second->operators.insert(std::make_pair(channels_iter->second->client.begin()->first, channels_iter->second->client.begin()->second));
						for(ClientIter joined = channels_iter->second->client.begin(); joined != channels_iter->second->client.end(); joined++)
						{
							//:mine!~t@197.230.30.146 MODE #abdoo +o mine
							std::string message =  ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username() + "@" + client_iter->second.get_clientip() + " MODE " + channels_iter->first + " +o " + channels_iter->second->client.begin()->second.get_client_nick() + "\r\n" ;
							send_message(joined->first, message);
						}
					}
				}
				else
					channels_iter->second->client.erase(cit);
				ClientIter op_it = channels_iter->second->operators.find(cit->first);
				if(op_it != channels_iter->second->operators.end())
					channels_iter->second->operators.erase(op_it);
				op_it = channels_iter->second->invited.find(cit->first);
				if(op_it != channels_iter->second->invited.end())
					channels_iter->second->invited.erase(op_it);
				channels_iter = listChannels.find(channelname);
				for (ClientIter it = channels_iter->second->client.begin(); it != channels_iter->second->client.end(); it++)
					send_message(it->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " :" + remind +  "\r\n");

					// send_message(it->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " :" + remind + "\r\n");
				if (channels_iter->second->client.size() == 0)
				{
					delete channels_iter->second;
					listChannels.erase(channels_iter);
				}
			}
			else
				send_message(fd, ":" + hostname + " 482 " + client_iter->second.get_client_nick() + " " + channelname + " :They aren't on that channel\r\n");
		}
		else
			send_message(fd, ":" + hostname + " 441 " + client_iter->second.get_client_nick() + " " + channelname + " :You're not a channel operator\r\n");
		}
	else
		send_message(fd, ":" + hostname + " 403 " + client_iter->second.get_client_nick() + " " + channelname + " :No such channel\r\n");

}
