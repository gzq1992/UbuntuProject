#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<signal.h>
#include<string.h>
#include<netdb.h>


int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("usage: %s ip port\n",argv[0]);
		exit(1);
	}
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}
	
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt,sizeof(opt));
	
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr.s_addr);
	
	printf("I will broadcast..\n");
	char* info = "hello iotek";
	size_t size = strlen(info) * sizeof(char);
	if(sendto(sockfd, info, size,  0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("sendto error");
		exit(1);
	}
	else
	{
		printf("broadcast success\n");
	}
	close(sockfd);
	
	return 0;
}














