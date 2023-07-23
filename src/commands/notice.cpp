#include "../server/server.hpp"

void	Server::notice(ClientIter client_iter, std::string remind, std::string command, int fd){
	std::stringstream	ss;
	std::string			name;
	VecStr				vec;
	std::string			msg;
	VecIter				nickname_it;
	channels			channel;
	ClientIter			client_it;
	bool				is_nickname = false;
	

	ss << remind;
	std::getline(ss, name, ' ');
	std::getline(ss, msg, '\0');
	vec = split(remind, ' ');
	nickname_it = std::find(nick_names.begin(), nick_names.end(), name);
	channelsIter channels_iter = listChannels.find(name);
	if(!msg.empty() && msg[0] != ':')
		msg = ":" + msg;
	if (nickname_it != nick_names.end())
	{
		for(client_it = clients_map.begin(); client_it != clients_map.end(); client_it++){
			if (client_it->second.get_client_nick() == name)
			{
				is_nickname = true;
				break;
			}
		}
		if (is_nickname && fd != client_it->first){
		std::string str = ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + client_it->second.get_client_nick() + " " + msg + "\r\n";
			send_message(client_it->first, str);
		}
	}
	else if (channels_iter != listChannels.end()){
		for(client_it = clients_map.begin(); client_it != clients_map.end(); client_it++){
			if (client_it->second.get_client_nick() == name)
				break;
		}
		for (ClientIter it = channels_iter->second->client.begin(); it != channels_iter->second->client.end(); it++)
		{
			if (it->first != fd){
				std::string str = ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + channels_iter->first + " " + msg + "\r\n";
				send_message(it->first , str);
			}
		}
}
}