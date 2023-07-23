#include "../server/server.hpp"

int	parce_nick(std::string &nick){
	for (size_t i = 0; i < nick.size(); i++){
		if (!std::isalnum(nick[i]))
			return (1);
	}
	return (0);
}


void	Server::check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, int fd){
	
	std::stringstream	ss;
	VecIter				nick_iter;
	std::string 		seminick;
	std::string			oldnick;
	UnregistredIter		un_iter;

	nick = remind;
	if (parce_nick(nick))
	{
		send_message(fd, ":" + hostname + " 432 NICK :Erroneus nickname\r\n");
		return ;
	}
	if (remind.empty()){
		send_message(fd, ":" + hostname + " 431 NICK :No nickname given\r\n");
		return ;
	}
	for(UnregistredIter it = un_names.begin(); it != un_names.end(); it++){
		if (it->second == nick && it->first != fd){
			send_message(fd, ":" + hostname + " 433  NICK :Is already in use.\r\n");
			return;
		}
	}
	client_iter->second.set_client_nick(nick);
	un_iter = un_names.find(fd);
	if (un_iter != un_names.end())
		un_names.erase(un_iter);
	un_names.insert(std::make_pair(fd, nick));

	if (!client_iter->second.get_registred())
		client_iter->second.increment_isvalid(command);
	else{
		oldnick = client_iter->second.get_client_nick();
		nick_iter = std::find(nick_names.begin(), nick_names.end(), oldnick);
		if (nick_iter != nick_names.end())
		{
			nick_names.erase(nick_iter);
			nick_names.push_back(nick);
		}
		std::string str = ":" + oldnick + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " :" + nick + "\r\n";
		if (nick != oldnick)
			send_message(fd,  str);
		for(channelsIter chiter = listChannels.begin(); chiter != listChannels.end(); chiter++)
		{
			ClientIter cit = chiter->second->client.find(fd);
			if (cit != chiter->second->client.end()){
				for (ClientIter it = chiter->second->client.begin(); it != chiter->second->client.end(); it++)
				{
					if (fd != it->first)
						send_message(it->first,  str);
					else
						it->second.set_client_nick(nick);

				}
			}
		}
		client_iter->second.set_client_nick(nick);
	}
}
