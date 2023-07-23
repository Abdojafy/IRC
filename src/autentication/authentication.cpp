#include "../server/server.hpp"

struct in_addr Server::get_clientip(int fd){
	ClientIter			client_iter;

	client_iter = clients_map.find(fd);
	return (client_iter->second.get_client_address().sin_addr);
}


std::string Server::set_welcome_msg(std::string hostname, ClientIter client_iter){
	
	std::string msg;
	msg = ":" + hostname + " 001 " + client_iter->second.get_client_nick() + " :Welcome to the IRC network, " + client_iter->second.get_client_nick() + "!\r\n";
	return msg;
}

void Server::client_isregistred(ClientIter client_iter, std::string hostname, int fd){
	std::string			welcome_msg;

	if (client_iter->second.get_isvalid() == 3 && !client_iter->second.get_registred()){
			nick_names.push_back(nick);
			client_iter->second.set_registred();
			client_iter->second.set_client_data(realname, pass);
			welcome_msg = set_welcome_msg(hostname, client_iter);
			send_message(fd, welcome_msg);
	}
}

int Server::exec_command(int fd)
{
	std::stringstream	ss;
	std::string			command;
	std::string			hostname;
	in_addr				addrip;
	std::string			remind;
	ClientIter			client_iter;

	client_iter = clients_map.find(fd);
	addrip = get_clientip(fd);
	hostname = inet_ntoa(addrip);
	client_iter->second.set_clientip(hostname);
	if (client_msg.empty())
		return 1;
	remove_new_line(client_msg);
	ss << client_msg;
	std::getline(ss, command, ' ');
	ft_upper(command);
	std::getline (ss, remind, '\0');
	remind = trim_spaces(remind);
	 if (!command.compare("NICK"))
		check_nickname(client_iter, remind, command, hostname, fd);
	else if (!command.compare("PASS"))
		check_pass(client_iter, remind, hostname, fd, command);
	else if (!command.compare("USER"))
		check_user(client_iter, remind, hostname, fd, command);
	client_isregistred(client_iter, hostname, fd);
	
	return 0;
}

