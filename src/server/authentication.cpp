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
	std::string 		seminick;
	std::string			oldnick;
	std::string			nick;
	std::string			pass;
	std::string			username;
	std::string			realname;
	std::string			command;
	std::string			hostname;
	in_addr				addrip;
	std::string			remind;
	VecIter				nick_iter;
	std::string			err_msg;
	VecStr				remindvec;
	ClientIter			client_iter;

	client_iter = clients_map.find(fd);
	client_msg = buffer;
	addrip = get_clientip(fd);
	hostname = inet_ntoa(addrip);
	err_msg = hostname + " 001 :Welcome to the ExampleNet Internet Relay Chat Network\n";	
	if (client_msg.empty())
		return 1;
	remove_new_line(client_msg);
	ss << client_msg;
	std::getline(ss, command, ' ');
	ft_upper(command);
	std::getline (ss, remind, '\0');
	ss.clear();
	ss << remind;
	remind = trim_spaces(remind);
	std::getline(ss, seminick, ' ');
	seminick = trim_spaces(seminick);

	if (!command.compare("NICK") && client_iter->second.get_registred()){
		oldnick = client_iter->second.get_client_nick();
		nick_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (nick_iter != nick_names.end())
			nick_names.erase(nick_iter);
		client_iter->second.set_client_nick(nick);
	}
	else if (!command.compare("PASS")){
		if (remind.empty())
		{
			err_msg = hostname + " 461 " + command + " :Not enough parameters\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		else if (remind.compare(password))
		{
			err_msg = hostname + " 464 chris :Password Incorrect\n";
			send(fd, err_msg.c_str(), err_msg.length(), 0);
		}
		else
		{
			pass = remind;
			client_iter->second.increment_isvalid();
		}
	}
	else if (!command.compare("NICK") && !client_iter->second.get_registred())
	{
		std::cout<<"helllo from nick"<<std::endl;
		nick_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (remind.empty()){
			err_msg = hostname + " 461 " + command + " :Not enough parameters\n";
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
			client_iter->second.increment_isvalid();
		}
	}
	else if (!command.compare("USER"))
	{
		remind = trim_spaces(remind);
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
			client_iter->second.increment_isvalid();
		}
	}

	std::cout<<client_iter->second.get_isvalid()<<std::endl;
	if (client_iter->second.get_isvalid() == 3 && !client_iter->second.get_registred()){
		nick_names.push_back(nick);
		client_iter->second.set_registred();
		client_iter->second.set_client_data(username, realname, pass, nick);
		send(fd, err_msg.c_str(), err_msg.length(), 0);
	}
	return 0;
}

