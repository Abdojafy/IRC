#include "client.hpp"

Client::Client()
{
	addrlen = sizeof(address);
}

int Client::start_client(){
	char buffer[30000] = {0};
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0){
		perror("socket");
		return (-1);
	}
	memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	if (inet_pton(AF_INET, "127.0.0.1",&address.sin_addr) <=  0)
	{
		perror("inet_pton");
		return (-1);
	}
	if (connect(client_fd, (struct sockaddr *)&address, addrlen) < 0)
	{
		perror("connect");
		return (-1);
	}
	send (client_fd, "hello from client", 17, 0);
	std::cout<<"hello from client sent"<<std::endl;
	int readresult = read(client_fd, buffer, 30000);
	std::cout<<buffer<<std::endl;
	return (0);
}


int main (){
	Client c;
	c.start_client();
}