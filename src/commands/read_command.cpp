#include"../server/server.hpp"

void	Server::read_command(PollIter it_client)
{
	VecStr command = split(client_msg, ' ');
	VecIter it = command.begin();
	if (it != command.end())
	{
		ft_upper(*it);
		if (!strcmp((*it).c_str(), "JOIN"))
		{
			command.erase(it);
			join(command, it_client);
		}
		if (!strcmp((*it).c_str(), "KICK"))
		{
			command.erase(it);
			// kick(command, it_client);
		}
	}
}