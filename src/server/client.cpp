#include "client.hpp"
Client::Client(){

}

Client::Client(sockaddr_in addr, int fd)
{
	address = addr;
	client_socket = fd;
}

Client::~Client()
{
}

sockaddr_in Client::get_client_address()
{
	return address;
}

Client &Client::operator=(const Client &other)
{
	int	i = 0;
	client_socket = other.client_socket;
	address.sin_addr = other.address.sin_addr;
	address.sin_family = other.address.sin_family;
	address.sin_len = other.address.sin_len;
	address.sin_port = other.address.sin_port;
	while(address.sin_zero[i]){
		address.sin_zero[i] = other.address.sin_zero[i];
		i++;
	}
	return (*this);
}
