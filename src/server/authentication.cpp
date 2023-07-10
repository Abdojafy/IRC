#include "server.hpp"

std::string remove_new_line(char *msg){
	int len = strlen(msg) - 1;
	if (msg[len] - 1 == '\r')
		msg[len - 1] = '\0';
	else
		msg[len] = '\0';
	return msg;
}

std::string	ft_upper(char *msg){
	int i = 0;
	while(msg[i]){
		if (std::islower(msg[i]))
			msg[i] -= 32;
		i++;
	}
	return (msg);
}

std::string Server::get_client_info()
{
	client_msg = buffer;
	if (client_msg.empty())
		return NULL;
	client_msg = remove_new_line(buffer);
	client_msg = ft_upper(buffer);
	return client_msg;
}

