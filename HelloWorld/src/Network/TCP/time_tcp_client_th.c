#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<signal.h>
#include<time.h>
#include <sys/types.h>
#include"msg.h"
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
	char buff[512];
	size_t size;
	char* prompt = ">";
	while(1)
	{
		memset(buff, 0, sizeof(buff));
		write(STDOUT_FILENO, prompt, 1);
		size = read(STDIN_FILENO, buff, sizeof(buff));
		if(size < 0)
			continue;
			buff[size - 1] = '\0';
			if(write_msg(sockfd, buff, sizeof(buff)) < 0)
			{
				perror("write msg error");
				continue;
			}
			else
			{
				if(read_msg(sockfd, buff, sizeof(buff)) < 0)
				{
					perror("read msg error");
					continue;
				}
				else
				{
					printf("%s\n",buff);
				}
			}
	}
	
	close(sockfd);
	return 0;
}






























