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

Server::Server(char **av)
{
	port = parce_port(av[1]);
	if (port == -1)
	{
		std::cout<<"Invalid Port"<<std::endl;
		exit(1);
	}
	adrlen = sizeof(addr_server);
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
	if (listen(server_socket, 1) < 0)
	{
		perror("listen");
		exit(1);
	}
    char buffer[bufferSize];
	const char* response;
	int recvresult;
	int sendresult;
	while (1){
		adrlen = sizeof(addr_client);
		client_socket = accept(server_socket, (struct sockaddr *)&addr_client, (socklen_t*)&adrlen);
		if (client_socket < 0)
		{
			perror("accept");
			exit (1);
		}
    	recvresult = recv(client_socket, buffer, bufferSize - 1, 0);
    	if (recvresult < 0) {
    	    perror("recv");
    	    exit(1);
    	}
    	response = "hello from server";
    	 sendresult= send(client_socket, response, strlen(response), 0);
    	if (sendresult < 0) {
    	    perror("send");
    	    exit(1);
    	}
    	close(client_socket);
	}
	close(server_socket);
}

Server::~Server()
{

}

