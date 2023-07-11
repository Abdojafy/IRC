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

std::vector<std::string> split(std::string str, char c){
	std::stringstream			ss;
	std::vector<std::string>	vec;
	std::string					tmp;
	
	ss << str;
	while(std::getline(ss, tmp, c))
		vec.push_back(tmp);
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

int Server::get_client_info(int fd)
{
	std::stringstream	ss;
	std::string			nick;
	std::string			tmp;
	std::string			user;
	std::string			command;
	std::string			inetntoa;
	in_addr				addrip;
	std::string			remind;
	VecIter				nick_iter;
	std::string			err_msg;
	VecStr				remindvec;
	bool				isregistred = false;
	bool				isvalid = false;

	
	client_msg = buffer;
	addrip = get_clientip(fd);
	inetntoa = inet_ntoa(addrip);
	if (client_msg.empty())
		return 1;
	remove_new_line(client_msg);
	ft_upper(client_msg);
	ss << client_msg;
	std::getline(ss, command, ' ');
	ss << tmp;
	std::getline (ss, remind, '\0');
	if (!command.compare("NICK"))
	{
		nick_iter = std::find(nick_names.begin(), nick_names.end(), remind);
		if (nick_iter != nick_names.end())
		{
			err_msg = inetntoa + "  433 * " + remind + " :Nickname is already in use.\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		else{
			if (*tmp.begin() == ':')
				nick = tmp;
			else
				nick = remind;
			nick_names.push_back(nick);
			err_msg = inetntoa + " 001 :Welcome to the ExampleNet Internet Relay Chat Network\n";
		}
		ss.clear();
	}
	if (!command.compare("USER"))
	{
		remind = trim_spaces(remind);
		remindvec = split(remind, ' ');
		if (remindvec.size() < 4){
			err_msg = inetntoa + " 461 " + command + " :Not enough parameters\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		if (isregistred)
		{
			err_msg = inetntoa + " 462 :You may not reregister\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
	}
	if (isvalid)
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	return 0;
}

