#include "server.hpp"

std::map<int, Client> Server::clients_map;
bool is_it_digits(std::string str){
	for (size_t i = 0; i < str.length(); i++){
		if  (std::isdigit(str[i]))
			return(false);
	}
	return (true);
}

int	parce_port(char *str){
	std::string port = str;
	if (is_it_digits(port))
		return (-1);
	int nb = atoi(port.c_str());
	if (nb < 0 || nb > 65535)
		return (-1);
	return (nb);
}

void Server::set_pass_and_port(char **av){
	port = parce_port(av[1]);
	password = av[2]; 
	if(password.empty()){
		std::cout<<"Please Enter A Valid Password"<<std::endl;
		exit(1);
	}
	if (port == -1)
	{
		std::cout<<"Invalid Port"<<std::endl;
		exit(1);
	}
}

void Server::send_message(int fd, std::string message)
{
	if (send(fd, message.c_str(), message.length(), 0) == -1)
	perror("send");
}

void Server::create_bind_listen(int port)
{
	int	option;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(1);
	}
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1){
		perror("setsocketopt");
		exit(1);
	}
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(port);
	if (bind(server_socket, (sockaddr *)&addr_server, sizeof(addr_server)) < 0)
	{
		perror("bind");
		exit (1);
	}
	if (listen(server_socket, 5) < 0)
	{
		perror("listen");
		exit(1);
	}
}

void Server::accept_new_client()
{
	Client client;
	client_addr_len = sizeof(addr_client);
	client_socket = accept(server_socket, (sockaddr *)&addr_client, (socklen_t*)&client_addr_len);
	if (client_socket < 0)
	{
		perror("accept");
		exit (1);
	}
	if (poll_fds.size() < MAXCLIENTS)
	{
		pollfd p;
		p.fd = client_socket;
		p.events = POLLIN;
		poll_fds.push_back(p);
		std::cout<<"new connection from client : "<<inet_ntoa(addr_client.sin_addr)<<"  in port : "<<htons(port)<<std::endl;
		send(client_socket, "wellcome to the server\n", 23, 0);
		client = Client(addr_client, client_socket);
		clients_map.insert(std::make_pair(client_socket, client));
	}
}

void Server::remove_client(int fd)
{
	ClientIter client_iter;
	VecIter		vec_iter;

	printf("Client disconnected\n");
	if (clients_map.size() > 0){
		client_iter = clients_map.find(fd);
		clients_map.erase(client_iter);
	}
	vec_iter = std::find(nick_names.begin(), nick_names.end(), nick);
	if (vec_iter != nick_names.end())
		nick_names.erase(vec_iter);
	for (PollIter pit = poll_fds.begin(); pit < poll_fds.end(); pit++){
		if(fd == pit->fd){
			poll_fds.erase(pit);
		}
	}
	close(fd);
}

void Server::read_client_data(PollIter it){

	if (it->revents & (POLLHUP | POLL_ERR))
		remove_client(it->fd);
	else if (it->revents & POLLIN){
		bzero(buffer, BUFFERSIZE - 1);
		int recv_len = read(it->fd, buffer, BUFFERSIZE - 1);
		buffer[recv_len] = '\0';
		client_msg += buffer;
		if (client_msg.find('\n') == std::string::npos)
			return;
		//ay haja rseltiha  mn lclient atl9aha fhad lbuffer les command dima ayb9aw wjiwkom hna b7all "pass kick" wa majawarahoma
		//hna fin t9dar tjawb lclient khdem bhad send li lta7t 3tiha it->fd o kteb lclient li bghiti
		get_client_info(it->fd);
		std::cout << "Received from client : " << client_msg << std::endl;
		send(it->fd, "Message received\n", 17, 0);
		client_msg.clear();
	}
}

Server::Server(char **av)
{
	set_pass_and_port(av);
	create_bind_listen(port);
	pollfd p;
	p.fd = server_socket;
	p.events = POLLIN;
	poll_fds.push_back(p);
	while (1)
	{
		poll_result = poll(poll_fds.data(), poll_fds.size(), -1);
		if (poll_result < 0){
			perror("poll");
			exit(1);
		}
		PollFds tmp = poll_fds;
		for (PollIter it = tmp.begin(); it != tmp.end() && poll_result > 0; it++){
			if (it->revents & POLLIN && it->fd == server_socket){
				accept_new_client();
				poll_result--;
			}else if(it->revents & POLLIN){
				read_client_data(it);
				poll_result--;
			}
		}
	}
	close(server_socket);
}

Server::~Server()
{

}
