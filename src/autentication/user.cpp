#include "../server/server.hpp"

void	Server::check_user(ClientIter client_iter, std::string remind, std::string hostname, int fd, std::string command){

	VecStr				remindvec;

	remindvec = split(remind, ' ');
	if (client_iter->second.get_registred())
		send_message(fd,  ":" + hostname + " 462 USER :You may not reregister\r\n");
	else if (remindvec.size() < 4)
		send_message(fd,  ":" + hostname + " 461 USER :Not enough parameters\r\n");
	else
	{
		username = *remindvec.begin();
		realname = *(remindvec.end() -1);
		client_iter->second.increment_isvalid(command);
	}
}