#include "client.hpp"
Client::Client(){

}

Client::Client(sockaddr_in addr, int fd)
{
	address = addr;
	client_socket = fd;	
}

void	Client::increment_isvalid(std::string command){

	if (command == "NICK" && !nickbool){
		isvalid++;
		nickbool = true;
	}
	else if (command == "USER" && !userbool){
		isvalid++;
		userbool = true;
	}
	else if (command == "PASS" && !passbool){
		isvalid++;
		passbool = true;
	}
}

int		Client::get_isvalid(){
	return isvalid;
}

int		Client::get_client_socket(){
	return client_socket;
}

void	Client::set_registred(){
	this->registred = true;
}

bool 	Client::get_registred(){
	return (registred);
}

Client::~Client()
{
}

sockaddr_in Client::get_client_address()
{
	return address;
}

std::string Client::get_client_nick(){
	return this->nick;
}

std::string Client::get_client_username(){
	return this->username;
}
std::string Client::get_client_realname(){
	return this->realname;
}

std::string Client::get_clientip(){
	return this->clientip;
}

void	Client::set_clientip(std::string ip){
	this->clientip = ip;
}

void Client::set_client_data(std::string username, std::string realname, std::string pass, std::string nick)
{
	this->username = username;
	this->realname = realname;
	this->pass = pass;
	this->nick = nick;
}

void Client::set_client_nick(std::string nick){
	this->nick = nick;
}

Client &Client::operator=(const Client &other)
{
	int	i = 0;
	registred = false;
	isvalid = 0;
	userbool = false;
	nickbool = false;
	passbool = false;
	client_socket = other.client_socket;
	address.sin_addr = other.address.sin_addr;
	address.sin_family = other.address.sin_family;
	// address.sin_len = other.address.sin_len;
	address.sin_port = other.address.sin_port;
	while(address.sin_zero[i]){
		address.sin_zero[i] = other.address.sin_zero[i];
		i++;
	}
	return (*this);
}
