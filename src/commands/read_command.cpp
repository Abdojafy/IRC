#include"../server/server.hpp"


//:zinc.libera.chat 421 youssef yopic :Unknown command
void	Server::read_command(PollIter it_client)
{
	VecStr command = split(client_msg, ' ');
	VecIter it = command.begin();
	std::string message;
	std::string unk_command;
	// ClientIter my_client = clients_map.find(it_client->fd);
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
			// kick(command, it_client);
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
		// else
		// {
		// 	message = ":" + my_client->second.get_clientip() + " 421 " + my_client->second.get_client_nick() + " " + unk_command + " :Unknown command\r\n"; 
		// 	send_message(it_client->fd, message);
		// 	return;
		// }
	}
}