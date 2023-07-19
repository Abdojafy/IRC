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
	channelsIter chunnels_iter = listChannels.find(name);
	if (vec.size() == 1)
		send_message(fd, ":" + client_iter->second.get_clientip() + " 412 " + client_iter->second.get_client_nick() + " " + name + " :No text to send\n\r");
	else if (nickname_it != nick_names.end() || chunnels_iter != listChannels.end())
	{
		for(client_it = clients_map.begin(); client_it != clients_map.end(); client_it++){
			if (client_it->second.get_client_nick() == name)
			{
				is_nickname = true;
				break;
			}
		}
		if (is_nickname)
			send_message(client_it->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\n\r");
		else{
			channel = chunnels_iter->second;
			for (ClientIter it = channel.client.begin(); it != channel.client.end(); it++);
			{
				send_message(it->first , ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\n\r");
			}
		}
	}
	else
		send_message(fd, ":" + client_iter->second.get_clientip() + " 401 " + client_iter->second.get_client_nick() + " " + name + " :No such nick/channel\r\n");
}