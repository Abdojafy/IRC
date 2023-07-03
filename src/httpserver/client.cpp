#include "client.hpp"

Client::Client(sockaddr_in addr, int fd)
{
	address = addr;
	client_socket = fd;
}

Client::~Client()
{
}
