#include "../Includes/ircserv.hpp"

void	start(string port, string password)
{
	(void)port;
	(void)password;
}

int	main(int ac, char **av)
{
	try {
		if (ac == 3)
			start(av[1], av[2]);
		else
			throw(runtime_error("Error"));
	}
	catch (exception &e) {
		cout << e.what() << endl;
	}
	return (0);
}