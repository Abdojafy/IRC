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


void	Server::check_nickname(ClientIter client_iter, std::string remind, std::string command, std::string hostname, std::string &err_msg , int fd){
	
	std::stringstream	ss;
	std::string			nick;
	VecIter				nick_iter;
	std::string 		seminick;
	std::string			oldnick;

	ss << remind;
	std::getline(ss, seminick, ' ');
	seminick = trim_spaces(seminick);

	if (!command.compare("NICK") && client_iter->second.get_registred()){
		oldnick = client_iter->second.get_client_nick();
		nick_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (nick_iter != nick_names.end())
			nick_names.erase(nick_iter);
		client_iter->second.set_client_nick(nick);
	}
	else if (!command.compare("NICK") && !client_iter->second.get_registred())
	{
		nick_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (remind.empty()){
			err_msg = hostname + " 431 :No nickname given\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		else if (nick_iter != nick_names.end())
		{
			err_msg = hostname + "  433 *  :Nickname is already in use.\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		else{
			if (*seminick.begin() == ':'){
				remind.erase(remind.begin());
				nick = remind;
			}
			else
				nick = seminick;
			client_iter->second.increment_isvalid(command);
		}
	}
}


void	Server::check_user(ClientIter client_iter, std::string remind, std::string hostname, std::string &err_msg, int fd, std::string command){

	VecStr				remindvec;

	remindvec = split(remind, ' ');
	if (client_iter->second.get_registred())
	{
		err_msg = hostname + " 462 :You may not reregister\n";
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	else if (remindvec.size() < 4){
		err_msg = hostname + " 461 " + command + " :Not enough parameters\n";
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	else
	{
		username = *remindvec.begin();
		realname = *remindvec.end();
		client_iter->second.increment_isvalid(command);
	}
}

void	Server::check_pass(ClientIter client_iter, std::string remind, std::string hostname, std::string &err_msg, int fd, std::string command){

	if (client_iter->second.get_registred())
	{
		err_msg = hostname + " 462 :You may not reregister\n";
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	else if (remind.empty())
	{
		err_msg = hostname + " 461 " + command + " :Not enough parameters\n";
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	else if (remind.compare(password))
	{
		std::cout<<remind<<std::endl;
		err_msg = hostname + " 464 :Password Incorrect\n";
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	else
	{
		pass = remind;
		client_iter->second.increment_isvalid(command);
	}
}


int Server::get_client_info(int fd)
{
	std::stringstream	ss;
	std::string			command;
	std::string			hostname;
	in_addr				addrip;
	std::string			remind;
	VecIter				nick_iter;
	std::string			err_msg;
	ClientIter			client_iter;

	client_iter = clients_map.find(fd);
	client_msg = buffer;
	addrip = get_clientip(fd);
	hostname = inet_ntoa(addrip);
	client_iter->second.set_clientip(hostname);
	err_msg = hostname + " 001 :Welcome to the ExampleNet Internet Relay Chat Network\n";	
	if (client_msg.empty())
		return 1;
	remove_new_line(client_msg);
	ss << client_msg;
	std::getline(ss, command, ' ');
	ft_upper(command);
	std::getline (ss, remind, '\0');
	remind = trim_spaces(remind);
	
	check_nickname(client_iter, remind, command, hostname, err_msg, fd);
	if (!command.compare("PASS"))
		check_pass(client_iter, remind, hostname, err_msg, fd, command);
	else if (!command.compare("USER"))
		check_user(client_iter, remind, hostname, err_msg, fd, command);
	if (client_iter->second.get_isvalid() == 3 && !client_iter->second.get_registred()){
		nick_names.push_back(nick);
		client_iter->second.set_registred();
		client_iter->second.set_client_data(username, realname, pass, nick);
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	return 0;
}

