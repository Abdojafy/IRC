#include "../server/server.hpp"


void	Server::check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, int fd){
	
	std::stringstream	ss;
	VecIter				nick_iter;
	std::string 		seminick;
	std::string			oldnick;

	ss << remind;
	std::getline(ss, seminick, ' ');
	seminick = trim_spaces(seminick);
	if (*seminick.begin() == ':'){
		remind.erase(remind.begin());
		nick = remind;
	}
	else
		nick = seminick;
	
	if (!command.compare("NICK") && !client_iter->second.get_registred())
	{
		nick_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (nick_iter != nick_names.end())
			send_message(fd, ":" + hostname + " 433 * NICK :Is already in use.\r\n");
		else
			client_iter->second.increment_isvalid(command);
	}
	if (remind.empty())
		send_message(fd, ":" + hostname + " 431 * NICK :No nickname given\r\n");
	else if (!command.compare("NICK") && client_iter->second.get_registred()){
		oldnick = client_iter->second.get_client_nick();
		nick_iter = std::find(nick_names.begin(), nick_names.end(), oldnick);
		if (nick_iter != nick_names.end())
			nick_names.erase(nick_iter);
		nick_names.push_back(nick);
		send_message(fd,  ":" + oldnick +"!" + hostname  + nick + "\r\n");
		client_iter->second.set_client_nick(nick);
	}
}
