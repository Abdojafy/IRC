#include"../server/server.hpp"


void	Server:: read_command(PollIter it_client)
{
	std::stringstream	ss;
	std::string			str_command;
	std::string			hostname;
	in_addr				addrip;
	std::string			remind;
	ClientIter			client_iter;

	client_iter = clients_map.find(it_client->fd);
	addrip = get_clientip(it_client->fd);
	hostname = inet_ntoa(addrip);
	client_iter->second.set_clientip(hostname);
	if (client_msg.empty())
		return ;
	remove_new_line(client_msg);
	ss << client_msg;
	std::getline(ss, str_command, ' ');
	std::getline (ss, remind, '\0');
	remind = trim_spaces(remind);
	
	VecStr command = split(client_msg, ' ');
	VecIter it = command.begin();
	std::string message;
	std::string unk_command;

	if (it != command.end())
	{
		unk_command = *it;
		ft_upper(*it);
		if (!strcmp((*it).c_str(), "JOIN"))
		{
			command.erase(it);
			join(command, it_client);
		}
		else if (!strcmp((*it).c_str(), "KICK"))
		{
			command.erase(it);
			kick(client_iter, remind, "KICK", it_client->fd, hostname);
		}
		else if (!strcmp((*it).c_str(), "INVITE"))
		{
			command.erase(it);
			invite(command, it_client);
		}
		else if (!strcmp((*it).c_str(), "TOPIC"))
		{
			command.erase(it);
			topic(command, it_client);
		}
		else if (!strcmp((*it).c_str(), "MODE"))
		{
			command.erase(it);
			modes(command, it_client);
		}
		else if (!client_msg.compare("QUIT :"))
			remove_client(it_client->fd);
		else if (!strcmp((*it).c_str(), "PRIVMSG"))
			privmsg(client_iter, remind, *it, it_client->fd);
		else if (!strcmp((*it).c_str(), "NOTICE"))
			notice(client_iter, remind, *it, it_client->fd);
		else if (!strcmp((*it).c_str(), "HELP"))
			boot_help(client_iter, remind, *it, it_client->fd);
		// else
		// {
		// 	message = ":" + my_client->second.get_clientip() + " 421 " + my_client->second.get_client_nick() + " " + unk_command + " :Unknown command\r\n"; 
		// 	send_message(it_client->fd, message);
		// 	return;
		// }
	}
}