#include "ircserv.hpp"

bool	is_digit(std::string str)
{
	std::string::iterator it;

	for (it = str.begin(); it != str.end(); it++) {
		if (!isdigit(*it))
			return (false);
	}
	return (true);
}

int	handle_port(std::string port_)
{
	int		save;

	if (port_.empty())
		throw(std::runtime_error("Error: port is empty."));
	else if (!is_digit(port_))
		throw(std::runtime_error("Error: port must have only digits."));
	save = std::stoi(port_);
	if (save <= 0 || save > 65535)
		throw(std::runtime_error("Error: maximum number of ports is 65535."));
	return (save);
}

bool	handle_password(std::string password_)
{
	if (password_.empty())
		throw(std::runtime_error("Error: password is empty."));
	else if (!is_digit(password_))
		return (false);
	return (true);
}
