#include "ircserv.hpp"
#include <stdexcept>
#include <sys/socket.h>

void	start(std::string port_, std::string password)
{
	int		port, s_socket;
	handle_port(port_), handle_password(password);
	port = std::stoi(port_);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (s_socket == -1)
		throw(std::runtime_error("Error: socket error."));
	std::cout << s_socket << std::endl;
}

int	main(int ac, char **av)
{
	try {
		if (ac == 3)
			start(av[1], av[2]);
		else
			throw(std::runtime_error("./ircserv <port> <password>"));
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}