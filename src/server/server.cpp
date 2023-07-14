#include "server.hpp"

bool	find_char(std::string str, char c)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

bool	Server::check_channel_name(std::string name)
{
	if (!name.empty() && name[0] == '#' && !find_char(name, ',') 
		&& !find_char(name, '\a') && name.length() > 1 && name.length() < 200)
	{
		return(true);
	}
	return(false);
}

void	Server::join_channels(PollIter it_client, std::string name, std::string pass)
{
	ClientIter it_map_client = clients_map.begin();
	channelsIter it_channel = listChannels.begin();
	Client my_client;
	channels *my_channel;
	std::string mode;
	std::string message;

	//which client
	while (it_map_client != clients_map.end())
	{
		if (it_map_client->first == it_client->fd)
		{
			my_client = it_map_client->second;
			break ;
		}
		it_map_client++;
	}
	if (it_map_client == clients_map.end())
	{
		std::cout << "error client not found" << std::endl;
		exit(1);
	}

	//which channel
	while (it_channel != listChannels.end())
	{
		if (it_channel->first == name)
		{
			my_channel = it_channel->second;
			break ;
		}
		it_channel++;
	}

	//check password in mode (+k)
	if (it_channel != listChannels.end())
	{
		if (strcmp(it_channel->second->get_password().c_str(), pass.c_str()))
		{
			message = ":server 475 " + my_client.get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+k)\n";
			send_message(it_client->fd, message);
			return;
		}
	}

	//add new channel with mode (o) 
	if (it_channel == listChannels.end())
	{
		mode += "o";
		if (!pass.empty())
			mode += "k";
		my_channel = new channels(name, pass, mode);
		my_channel->operators.insert(std::pair<int, Client>(it_map_client->first, my_client));
	}

	//check limit in mode (+l)
	if ((my_channel->get_mode()).find("l") == std::string::npos)
	{
		if (my_channel->client.size() > my_channel->get_limite())
		{
			message = ":server 471 " + my_client.get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+l)\n";
			send_message(it_client->fd, message);
			return;
		}
	}

	//check invite-only in mode (+i)
	if ((my_channel->get_mode()).find("i") == std::string::npos)
	{
		if (!my_channel->get_invited())
		{
			message = ":server 473 " + my_client.get_client_nick() + " " + it_channel->second->get_name() + " :Cannot join channel (+i)\n";
			send_message(it_client->fd, message);
			return;
		}
	}
	// insert a channel successfully
	my_channel->client.insert(std::pair<int, Client>(it_client->fd, my_client));
	listChannels.insert(std::pair<std::string, channels *>(name, my_channel));
	message = ":server 353 " + my_client.get_client_nick() + " " + name +" :@" + my_channel->get_users() + "\n";
	send_message(it_client->fd, message);
}

void	Server::join(VecStr command, PollIter it_client)
{
	VecIter it = command.begin();
	ClientIter my_client_it = clients_map.begin();
	std::string pass;
	std::string name;
	std::string message;
	std::istringstream names;
	std::istringstream passwords;
	
	//which client
	while (my_client_it != clients_map.end())
	{
		if (my_client_it->first == it_client->fd)
			break;
		my_client_it++;
	}
	//error if no client (possible ctrl + C)
	if (my_client_it == clients_map.end())
	{
		std::cout << "error no client " << std::endl;
		exit(1);
	}
	//check params and join
	if (command.size() < 3 && !command.empty())
	{
		names.str(*it++);
		if (it != command.end())
			passwords.str(*it);
		while (std::getline(names, name,','))
		{
			std::getline(passwords, pass,',');
			if (check_channel_name(name))
			{
				channelsIter itch = listChannels.begin();
				while (itch != listChannels.end())
				{
					if (itch->first == name)
					{
						join_channels(it_client, name, pass);
						break ;
					}
					itch++;
				} 
				if (itch == listChannels.end())
				{
					join_channels(it_client, name, pass);
				}
			}
			else
			{
				message = ":server 403 " + my_client_it->second.get_client_nick() + " " + name + " :No such channel\n";
				send_message(it_client->fd, message);
			}
			pass.clear();
		}
	}
	else
	{
		message = ":server 461 " + my_client_it->second.get_client_nick() + " JOIN :Not enough parameters\n";
		send_message(it_client->fd, message);
	}
}

void	Server::read_command(PollIter it_client)
{
	VecStr command = split(client_msg, ' ');
	VecIter it = command.begin();
	ft_upper(*it);
	if (!strcmp((*it).c_str(), "JOIN"))
	{
		command.erase(it);
		join(command, it_client);
	}
}

bool is_it_digits(std::string str){
	for (size_t i = 0; i < str.length(); i++){
		if  (std::isdigit(str[i]))
			return(false);
	}
	return (true);
}

int	parce_port(char *str){
	std::string port = str;
	if (is_it_digits(port))
		return (-1);
	int nb = atoi(port.c_str());
	if (nb < 0 || nb > 65535)
		return (-1);
	return (nb);
}

void Server::set_pass_and_port(char **av){
	port = parce_port(av[1]);
	password = av[2]; 
	if(password.empty()){
		std::cout<<"Please Enter A Valid Password"<<std::endl;
		exit(1);
	}
	if (port == -1)
	{
		std::cout<<"Invalid Port"<<std::endl;
		exit(1);
	}
}

void Server::send_message(int fd, std::string message)
{
	if (send(fd, message.c_str(), message.length(), 0) == -1)
	perror("send");
}

void Server::create_bind_listen(int port)
{
	int	option;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(1);
	}
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1){
		perror("setsocketopt");
		exit(1);
	}
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(port);
	if (bind(server_socket, (sockaddr *)&addr_server, sizeof(addr_server)) < 0)
	{
		perror("bind");
		exit (1);
	}
	if (listen(server_socket, 5) < 0)
	{
		perror("listen");
		exit(1);
	}
}

void Server::accept_new_client()
{
	Client client;
	client_addr_len = sizeof(addr_client);
	client_socket = accept(server_socket, (sockaddr *)&addr_client, (socklen_t*)&client_addr_len);
	if (client_socket < 0)
	{
		perror("accept");
		exit (1);
	}
	if (poll_fds.size() < MAXCLIENTS)
	{
		pollfd p;
		p.fd = client_socket;
		p.events = POLLIN;
		poll_fds.push_back(p);
		std::cout<<"new connection from client : "<<inet_ntoa(addr_client.sin_addr)<<"  in port : "<<htons(port)<<std::endl;
		send(client_socket, "wellcome to the server\n", 23, 0);
		client = Client(addr_client, client_socket);
		clients_map.insert(std::make_pair(client_socket, client));
	}
}

void Server::read_client_data(PollIter it){
	ClientIter client_iter;
	VecIter		vec_iter;

	if (it->revents & (POLLHUP | POLL_ERR)){
		printf("Client disconnected\n");
		if (clients_map.size() > 0){
			client_iter = clients_map.find(it->fd);
			clients_map.erase(client_iter);
		}
		vec_iter = std::find(nick_names.begin(), nick_names.end(), nick);
		if (vec_iter != nick_names.end())
			nick_names.erase(vec_iter);
		for (PollIter pit = poll_fds.begin(); pit < poll_fds.end(); pit++){
			if(it->fd == pit->fd){
				poll_fds.erase(pit);
			}
		}
		close(it->fd);
	}
	else if (it->revents & POLLIN){
		bzero(buffer, BUFFERSIZE - 1);
		int recv_len = read(it->fd, buffer, BUFFERSIZE - 1);
		buffer[recv_len] = '\0';
		rest += buffer;
		if (rest.find('\n') == std::string::npos)
			return;
		//ay haja rseltiha  mn lclient atl9aha fhad lbuffer les command dima ayb9aw wjiwkom hna b7all "pass kick" wa majawarahoma
		//hna fin t9dar tjawb lclient khdem bhad send li lta7t 3tiha it->fd o kteb lclient li bghiti
		get_client_info(it->fd);
		std::cout << "Received from client : " << client_msg << std::endl;
		read_command(it);

	// **** show map of channels ****
	//{

		// ClientIter client_iter;
		// ClientIter operator_iter;
		// channelsIter itc = listChannels.begin();
		// while (itc != listChannels.end())
		// {
		// 	client_iter = itc->second->client.begin();
		// 	std::cout << "*	channel [" << itc->second->get_name() << "] has key (" << itc->second->get_password() << ") modes = " << itc->second->get_mode() <<"| with this clients {";
		// 	while (client_iter != itc->second->client.end())
		// 	{
		// 		operator_iter = itc->second->operators.begin();
		// 		while (operator_iter != itc->second->operators.end())
		// 		{
		// 			if (operator_iter->first == client_iter->first)
		// 				std::cout << " Operator " ;
		// 			operator_iter++;
		// 		}
		// 		std::cout << client_iter->second.get_client_socket() << ", ";
		// 		client_iter++;
		// 	}
		// 	std::cout <<"}"<<std::endl;
		// 	itc++;
		// }
	//}	

		send(it->fd, "Message received\n", 17, 0);
	}
}

Server::Server(char **av)
{
	set_pass_and_port(av);
	create_bind_listen(port);
	pollfd p;
	p.fd = server_socket;
	p.events = POLLIN;
	poll_fds.push_back(p);
	while (1)
	{
		poll_result = poll(poll_fds.data(), poll_fds.size(), -1);
		if (poll_result < 0){
			perror("poll");
			exit(1);
		}
		PollFds tmp = poll_fds;
		for (PollIter it = tmp.begin(); it != tmp.end() && poll_result > 0; it++){
			if (it->revents & POLLIN && it->fd == server_socket){
				accept_new_client();
				poll_result--;
			}else if(it->revents & POLLIN){
				read_client_data(it);
				poll_result--;
			}
		}
	}
	close(server_socket);
}

Server::~Server()
{

}
