#include "../server/server.hpp"

void	Server::privmsg(ClientIter client_iter, std::string remind, std::string command, int fd){

	std::stringstream	ss;
	std::string			name;
	VecStr				vec;
	VecIter				nickname_it;
	channels			channel;
	ClientIter			client_it;
	bool				is_nickname = false;

	ss << remind;
	std::getline(ss, name, ' ');
	vec = split(remind, ' ');
	nickname_it = std::find(nick_names.begin(), nick_names.end(), name);
	channelsIter channels_iter = listChannels.find(name);
	if (vec.size() == 1)
		send_message(fd, ":" + client_iter->second.get_clientip() + " 412 " + client_iter->second.get_client_nick() + " " + name + " :No text to send\r\n");
	else if (nickname_it != nick_names.end() || channels_iter != listChannels.end())
	{
		for(client_it = clients_map.begin(); client_it != clients_map.end(); client_it++){
			if (client_it->second.get_client_nick() == name)
			{
				is_nickname = true;
				break;
			}
		}
		if (is_nickname){
			std::cout<<client_it->first<<" helllll-----------------------------"<<std::endl;
			send_message(client_it->first, ":" + client_iter->second.get_client_nick() + " " + command + " " + client_it->second.get_client_nick() + " :" + remind + "\r\n");
		}
		else{
			for (ClientIter it = channels_iter->second->client.begin(); it != channels_iter->second->client.end(); it++)
			{
				if (it->first != fd){
					
					send_message(it->first , ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\r\n");
				}
			}
		}
	}
	else
		send_message(fd, ":" + client_iter->second.get_clientip() + " 401 " + client_iter->second.get_client_nick() + " " + name + " :No such nick/channel\r\n");
}