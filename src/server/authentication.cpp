#include "server.hpp"

void remove_new_line(std::string &msg){
	int pos = -1;
	pos = msg.find('\r');
	if (pos != -1)
		msg.erase(pos);
	msg.erase(msg.length() - 1);
}

void	ft_upper(std::string &msg){
	int i = 0;
	while(msg[i]){
		if (std::islower(msg[i]))
			msg[i] -= 32;
		i++;
	}
}

int Server::get_client_info(int fd)
{
	std::stringstream	ss;
	std::string			command;
	ClientNicIter		nick_iter;
	std::string			err_msg;
	client_msg = buffer;
	if (client_msg.empty())
		return 1;
	remove_new_line(client_msg);
	ft_upper(client_msg);
	ss << client_msg;
	std::getline(ss, command, ' ');
	if (!command.compare("NICK"))
	{
		nick_iter = std::find(nick_names.begin(), nick_names.end(), ss.str());
		if (nick_iter != nick_names.end())
			err_msg = ":irc.example.com 433 * ben :Nickname is already in use.";
		else
		{
			nick_names.push_back(ss.str());
			err_msg = ":irc.example.com 001 dan :Welcome to the ExampleNet Internet Relay Chat Network dan";
		}
		send(fd, err_msg.c_str(), err_msg.length(), 0);
		ss.clear();
	}
	if (!command.compare("USER"))
	
	{
	return 0;
}

