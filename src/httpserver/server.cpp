#include "server.hpp"

Server::Server()
{
	adrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		exit(1);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(8080);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	if (bind(server_fd, (struct sockaddr *)&address, adrlen) < 0)
	{
		perror("bind");
		exit (1);
	}
	if (listen(server_fd, 1) < 0)
	{
		perror("listen");
		exit(1);
	}
	while (1){
		std::cout<<"-------waiting-------"<<std::endl;
		acpt = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&adrlen);
		if (acpt < 0)
		{
			perror("accept");
			exit (1);
		}
		char buffer[30000] = {0};
		readresult = recv(acpt, buffer, 30000, 0);
		std::cout<<"Client Says : "<<buffer<<std::endl;
		write(acpt, "Server Recived", 17);
		close(acpt);
	}
}

Server::~Server()
{

}

int main (){
	Server a;
}