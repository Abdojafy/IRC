#include "ircserv.hpp"

int main(int ac, char **av){
	(void) av;
	if (ac != 3)
	{
		std::cerr<<"Invalid Arguments"<<std::endl;
		exit(1);
	}
	int fd;
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0){
		perror("socket");
		exit(1);
	}
	int reuse = 0;
	int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if (opt < 0)
	{
		perror("setsockopt");
		exit(1);
	}
	
	struct sockaddr_in addr;
	addr.sin_family  = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	int bindresult = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (bindresult < 0)
	{
		perror("bind");
		exit(1);
	}
	


}