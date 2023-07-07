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
	int	clients = 1;
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
	while (1){
		poll_fd = poll(poll_fds.data(), clients, -1);
		if (poll_fd < 0){
			perror("poll");
			exit(1);
		}
		for (PollIter it = poll_fds.begin(); it < poll_fds.end(); ++it){
				if (first_fd->revents == POLLIN){
					adrlen = sizeof(addr_client);
					client_socket = accept(server_socket, (struct sockaddr *)&addr_client, (socklen_t*)&adrlen);
					if (client_socket < 0)
					{
						perror("accept");
						exit (1);
					}
					if (clients < MAXCLIENTS)
					{
						poll_fds.data()[clients].events = POLLIN;
						poll_fds.data()[clients].fd  = client_socket;
						poll_fds.push_back(poll_fds.data()[clients]);
						clients++;
						std::cout<<"new connection from client : "<<inet_ntoa(addr_client.sin_addr)<<"  in port : "<<htons(port)<<std::endl;
						send(client_socket, "wellcome to the server\n", 23, 0);
						clients_map.insert(std::make_pair(client_socket, Client(addr_client, client_socket)));
					}
				}
				int recv_len = recv(client_socket, buffer, BUFFERSIZE, 0);
                if (it->revents == POLLHUP) {
					it->events = 0;
					client_iter = clients_map.find(client_socket);
					clients_map.erase(client_iter);
					for (PollIter it = poll_fds.begin() + 1; it < poll_fds.end(); it++){
						if(it->fd == client_socket)
							poll_fds.erase(it);
					}
                    printf("Client disconnected\n");
                    close(client_socket);
				}
				else{
					buffer[recv_len] = '\0';
                    printf("Received from client : %s", buffer);
                    send(client_socket, "Message received\n", 17, 0);
				}
			}
	}
	close(server_socket);
}

Server::~Server()
{

}
