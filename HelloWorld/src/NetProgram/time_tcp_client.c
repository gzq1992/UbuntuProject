#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<signal.h>
#include<time.h>

int sockfd;


int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("usage: %s ip port\n",argv[0]);
		exit(1);
	}
	
	//step 1: creat socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}
	
	//fill ip, port and address type(IPV4) in serveraddr
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr.s_addr);
	
	//step 2: Client calls connect function to connect to server 
	if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}
	
	//step 3: call I/O functions(read,write) to communicate with server
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	size_t size;
	if((size = read(sockfd, buffer, sizeof(buffer))) < 0)
	{
		perror("read error");
	}
	if(write(STDOUT_FILENO, buffer, size) != size)
	{
		perror("write error");
	}
	close(sockfd);
	return 0;
}






























