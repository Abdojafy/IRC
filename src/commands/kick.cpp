#include"../server/server.hpp"

void	Server::kick(ClientIter client_iter, std::string remind, std::string command, int fd, std::string hostname)
{
	// :mercury.libera.chat 482 mzarh #ccoo :You're not a channel operator
		// :zinc.libera.chat 461 mzarhou KICK :Not enough parameters
		// :mzarhou!~o@197.230.30.146 KICK #ccoo mzarh :mzarh
		// :zinc.libera.chat 441 mzarhou jjj #ccoo :They aren't on that channel
		// :zinc.libera.chat 403 mzarhou ieh :No such channel

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
		std::getline(ss, clientname, '\0');
		channelsIter channels_iter = listChannels.find(channelname);
		if (vec.size() != 2)
			send_message(fd, ":" + hostname + " 461 " + client_iter->second.get_client_nick() + " " + command + " :Not enough parameters\r\n");
		else if (channels_iter != listChannels.end()){
			for (ClientIter it = channels_iter->second->operators.begin(); it != channels_iter->second->operators.end(); it++){
				if (it->second.get_client_nick() == client_iter->second.get_client_nick()){
					is_admin = true;
					break;
				}
			}
			if (is_admin){
				for (cit = channels_iter->second->client.begin(); cit != channels_iter->second->client.end(); cit++){
					if (cit->second.get_client_nick() == clientname){
						is_nickname = true;
						break;
					}
				}
				if (is_nickname){
					if (channels_iter->second->client.size() == 1)
					{
						channels_iter->second->client.erase(cit);
						listChannels.erase(channels_iter);
					}
					else if (cit->first == fd && channels_iter->second->operators.size() == 1)
					{
						channels_iter->second->client.erase(cit);
						channels_iter = listChannels.find(channelname);
						ClientIter it1 = channels_iter->second->client.begin();
						channels_iter->second->operators.insert(std::make_pair(it1->first, it1->second));
					}
					else
						channels_iter->second->client.erase(cit);

					channels_iter = listChannels.find(channelname);
					for (ClientIter it = channels_iter->second->client.begin(); it != channels_iter->second->client.end(); it++)
						send_message(it->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\n\r");
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
