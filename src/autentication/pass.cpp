#include "../server/server.hpp"

void	Server::check_pass(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command){

	if (client_iter->second.get_registred())
		send_message(fd, ":" + hostname + " 462 PASS :You may not reregister\r\n");
	else if (remind.empty())
		send_message(fd, ":" + hostname + " 461 " + command + "PASS :Not enough parameters\r\n");
	else if (remind.compare(password))
		send_message(fd, ":" + hostname + " 464 PASS :Password Incorrect\r\n");
	else
	{
		pass = remind;
		client_iter->second.increment_isvalid(command);
	}
}
