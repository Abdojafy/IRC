#include "../server/server.hpp"

std::string Server::help_msg(ClientIter client_iter){
	std::string str;
	str = ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Availabel Commands :  [join]    [privmsg]    [notice]    [kick]\r\n";
	str += ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :                      [user]  [topic] [invite]  [nick]  [pass]\r\n";
	str += ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :How to use         :  help + one of the above commands\r\n";
	str += ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Example            :  help join\r\n";
	return (str);
}

void Server::boot_help(ClientIter client_iter, std::string remind, std::string command, int fd){
	std::string msg = help_msg(client_iter);

	if (!command.compare("HELP") && !remind.compare(""))
		send_message(fd, msg);
	else if (!command.compare("HELP") && !remind.compare("privmsg"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to send private msg to a client or channel Syntax = (privmsg + name + msg)\r\n");
	else if (!command.compare("HELP") && !remind.compare("notice"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to notice a client or a channel Syntax = (notice + name + msg)\r\n");
	else if (!command.compare("HELP") && !remind.compare("kick"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to if you are Admin to kick someone from channel Syntax = (kick + #channelName  + nickName)\r\n");
	else if (!command.compare("HELP") && !remind.compare("join"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to join a channel or creat it if it doesn't exist Syntax = (join + #channelName)\r\n");
	else if (!command.compare("HELP") && !remind.compare("nick"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to set your nick name Syntax = (nick + nickName)\r\n");
	else if (!command.compare("HELP") && !remind.compare("user"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to set your user name Syntax = (user + username one two realname\r\n");
	else if (!command.compare("HELP") && !remind.compare("pass"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to access to the server Syntax = (pass + password(that setted by the server)\r\n");
	else if (!command.compare("HELP") && !remind.compare("topic"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to change discreption of channel Syntax = (topic +  #channelName + new topic\r\n");
	else if (!command.compare("HELP") && !remind.compare("invite"))
		send_message(fd, ":BOT PRIVMSG " + client_iter->second.get_client_nick() + " :Allows you to invite someone to a specific channel Syntax = (topic +  #channelName + new topic\r\n");
}