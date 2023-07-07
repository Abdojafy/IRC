#include "server.hpp"

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

void Server::create_bind_listen(int port)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(1);
	}
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(port);
	if (bind(server_socket, (struct sockaddr *)&addr_server, sizeof(addr_server)) < 0)
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

int Server::get_clientnumber()
{
	int len = 0;
	
	for(PollIter it = poll_fds.begin(); it < poll_fds.end(); it++)
		len++;
	return (len);
}

Server::Server(char **av)
{
	PollIter	first_fd;
	ClientIter	client_iter;
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
	create_bind_listen(port);
	fds[0].fd = server_socket;
	fds[0].events = POLLIN;
	poll_fds.push_back(fds[0]);
	first_fd = poll_fds.begin();
	int	clients = 1;
	while (1){
		std::cout << "hello there : " << poll_fds.size() << std::endl;
		poll_result = poll(poll_fds.data(), clients, -1);
		if (poll_result < 0){
			perror("poll");
			exit(1);
		}
		PollFds tmp = poll_fds;
		for (PollIter it = tmp.begin(); it != tmp.end(); it++){
			std::cout << it->revents << " " << it->fd << std::endl;
				if (it->revents == POLLIN && it->fd == server_socket){
					client_addr_len = sizeof(addr_client);
					client_socket = accept(server_socket, (struct sockaddr *)&addr_client, (socklen_t*)&client_addr_len);
					if (client_socket < 0)
					{
						perror("accept");
						exit (1);
					}
					if (clients < MAXCLIENTS)
					{
						pollfd p;
						p.fd = client_socket;
						p.events = POLLIN;
						poll_fds.push_back(p);
						clients++;
						std::cout<<"new connection from client : "<<inet_ntoa(addr_client.sin_addr)<<"  in port : "<<htons(port)<<std::endl;
						send(client_socket, "wellcome to the server\n", 23, 0);
						clients_map.insert(std::make_pair(client_socket, Client(addr_client, client_socket)));
					}
				}else{
					if ( it->revents == (POLLHUP | POLLERR)) {
						it->events = 0;
						printf("Client disconnected\n");
						close(client_socket);
						client_iter = clients_map.find(client_socket);
						clients_map.erase(client_iter);
						// poll_fds.erase(it);
						for (PollIter it = poll_fds.begin(); it < poll_fds.end(); it++){
							if(it->fd == client_socket)
								poll_fds.erase(it);
						}
					}
					else if (it->revents == POLLIN){
						int recv_len = recv(it->fd, buffer, BUFFERSIZE, 0);
						buffer[recv_len] = '\0';
						printf("Received from client : %s", buffer);
						send(client_socket, "Message received\n", 17, 0);
					}
				}
				std::cout << "hey " << std::endl;
			}
	}
	close(server_socket);
}

Server::~Server()
{

}
