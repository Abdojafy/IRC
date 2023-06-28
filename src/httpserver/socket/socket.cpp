#include "socket.hpp"

Socket::Socket(int domain, int type, int protocol, int port, u_long ip)
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("socket");
		exit(1);
	}
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(ip);
	connection = connect_network(socket_fd, address);
}


Socket::~Socket()
{

}

sockaddr_in Socket::get_address()
{
	return address;
}

int Socket::get_socketfd()
{
	return socket_fd;
}

int Socket::get_connection()
{
	return connection;
}

void Socket::set_connection(int connect)
{
	connection = connect;
}
