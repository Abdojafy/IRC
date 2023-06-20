#include <iostream>
#include <exception>
#include <sys/socket.h>
#include <_ctype.h>
#include <stdexcept>
#include <string>

bool	is_digit(std::string str);
int		handle_port(std::string port_);
bool	handle_password(std::string password_);