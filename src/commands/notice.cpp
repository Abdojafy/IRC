#include "../server/server.hpp"

void	Server::notice(ClientIter client_iter, std::string remind, std::string command, int fd){

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
	if (nickname_it != nick_names.end() || channels_iter != listChannels.end())
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
			for (ClientIter it = channels_iter->second->client.begin(); it != channels_iter->second->client.end(); it++){
				if (it->first != fd)
					send_message(it->first , ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\r\n");
			}		
		}
	}
}