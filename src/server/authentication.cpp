#include "server.hpp"

void remove_new_line(std::string &msg){
	int pos = -1;
	pos = msg.find('\r');
	if (pos != -1)
		msg.erase(pos);
	pos = msg.find('\n');
	if (pos != -1)
		msg.erase(pos);

}

void	ft_upper(std::string &msg){
	int i = 0;
	while(msg[i]){
		if (std::islower(msg[i]))
			msg[i] -= 32;
		i++;
	}
}

std::vector<std::string> split(std::string str, char c){
	std::stringstream			ss;
	std::vector<std::string>	vec;
	std::string					tmp;
	
	ss << str;
	while(std::getline(ss, tmp, c))
	{
		if (tmp[0] != 0)
			vec.push_back(tmp);
	}
	return (vec);
}

std::string trim_spaces(std::string str){
	while(1){
		if (std::isspace(str.front()))
			str.erase(0, 1);
		if (std::isspace(str.back()))
			str.erase(str.length() - 1, str.length());
		else if(!std::isspace(str.front()) && !std::isspace(str.back()))
			break;
	}
	return (str);
}

struct in_addr Server::get_clientip(int fd){
	ClientIter			client_iter;

	client_iter = clients_map.find(fd);
	
	return (client_iter->second.get_client_address().sin_addr);
}


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


void	Server::check_user(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command){

	VecStr				remindvec;

	remindvec = split(remind, ' ');
	if (client_iter->second.get_registred())
		send_message(fd,  ":" + hostname + " 462 USER :You may not reregister\r\n");
	else if (remindvec.size() < 4)
		send_message(fd,  ":" + hostname + " 461 USER :Not enough parameters\r\n");
	else
	{
		username = *remindvec.begin();
		realname = *(remindvec.end() -1);
		client_iter->second.increment_isvalid(command);
	}
}

void	Server::check_pass(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command){

	if (client_iter->second.get_registred())
		send_message(fd, ":" + hostname + " 462 PASS :You may not reregister\r\n");
	else if (remind.empty())
		send_message(fd, ":" + hostname + " 461 " + command + "PASS :Not enough parameters\r\n");
	else if (remind.compare(password))
		send_message(fd, ":" + hostname + "464 PASS :Password Incorrect\r\n");
	else
	{
		pass = remind;
		client_iter->second.increment_isvalid(command);
	}
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
		client_iter->second.set_client_data(username, realname, pass, nick);
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
	if (!client_msg.compare("QUIT :"))
		remove_client(fd);
	if (!command.compare("NICK"))
		check_nickname(client_iter, remind, command, hostname, fd);
	else if (!command.compare("PASS"))
		check_pass(client_iter, remind, hostname, fd, command);
	else if (!command.compare("USER"))
		check_user(client_iter, remind, hostname, fd, command);
	client_isregistred(client_iter, hostname, fd);
	if (!command.compare("PRIVMSG"))
	{
		std::stringstream	ss;
		std::string			name;
		VecStr				vec;
		VecIter				nickname_it;
		ClientIter			client_it;
		ss << remind;
		std::getline(ss, name, ' ');
		vec = split(remind, ' ');
		nickname_it = std::find(nick_names.begin(), nick_names.end(), name);
		if (vec.size() == 1)
			send_message(fd, ":" + client_iter->second.get_clientip() + " 412 " + client_iter->second.get_client_nick() + " " + name + " :No text to send\n\r");
		else if (nickname_it != nick_names.end())
		{
			for(client_it = clients_map.begin(); client_it != clients_map.end(); client_it++){
				if (client_it->second.get_client_nick() == name)
					break;
			}
			send_message(client_it->first, ":" + client_iter->second.get_client_nick() + "!~" + client_iter->second.get_client_username()+ "@" + client_iter->second.get_clientip() + " " + command + " " + remind + "\n\r");
		}
		else
			send_message(fd, ":" + client_iter->second.get_clientip() + " 401 " + client_iter->second.get_client_nick() + " " + name + " :No such nick/channel\n\r");

	}
	return 0;
}

