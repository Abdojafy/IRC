#include "../server/server.hpp"

std::string Server::help_msg(std::string hostname){
	std::string str;
	(void) hostname;
	str = ":user: 300 * BOT :CHECKNICK (ERROR) No nick given\r\n";
	// str += "\r\n";
	// str += "How to use: help + COMMAND\r\n";
	// str += "\r\n";
	// str += ":example:\r\n";
	// str += ":            help privmsg\r\n";
	// str += ":            help notice\r\n";
	// str += ":            help kick\r\n";
	// str += ":            help join . . .\r\n";
	return (str);
}

void Server::boot_help(std::string hostname, int fd){
	std::string msg = help_msg(hostname);

	if (!client_msg.compare("help"))
		send_message(fd, msg);
	else if (!client_msg.compare("help privmsg"))
		send_message(fd, "300 * :Allows you to send message to a client or a channel\r\n:Syntax:\r\n:privmsg + Client nickname(receiver nick name) or channel + Message\r\n");
	else if (!client_msg.compare("help notice"))
		send_message(fd, ":Allows you to notice a client or a channel\r\n:Syntax:\r\n:notice + Client nickname(receiver nick name) or channel + Message\r\n");
	else if (!client_msg.compare("help kick"))
		send_message(fd, ":Allows you to if you are Admin to kick someone of channel\r\n:Syntax:\r\n:kick + channel name (add # before the channel's name) + nick name(of the client that you want to kick)\r\n");
	else if (!client_msg.compare("help join"))
		send_message(fd, ":Allows you to join a channel or creat it if it doesn't exist\r\n:Syntax:\r\n:join + channel name (add # before the channel's name) + password (if you want to set a password to it) or (if you want to join private channel)\r\n");
	else if (!client_msg.compare("help nick"))
		send_message(fd, ":Allows you to set your nick name \r\n:Syntax:\r\n:nick + your nick name\r\n");
	else if (!client_msg.compare("help user"))
		send_message(fd, ":Allows you to set your user name \r\n:Syntax:\r\n:user + (it needs four argument the most important ones are the first and the last)\r\n");
	else if (!client_msg.compare("help pass"))
		send_message(fd, ":Allows you to access to the server \r\n:Syntax:\r\n:pass + password(that setted by the server)\r\n");
	else if (!client_msg.compare("help topic"))
		send_message(fd, ":Allows you to change discreption of channel\r\n:Syntax:\r\n:topic +  #channel name + new topic");
}