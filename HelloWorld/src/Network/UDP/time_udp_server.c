#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>

int sockfd;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("server close\n");
		close(sockfd);
		exit(1);
	}
}

void out_addr(struct sockaddr_in* clientaddr)
{
	int port = ntohs(clientaddr->sin_port);
	char ip[16];
	memset(ip,0,sizeof(ip));
	inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));
	printf("client: %s(%d) connected\n", ip, port);
}

void do_service()
{
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	//receive data from client
	if(recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len) < 0)
	{
		perror("recvfrom error");
	}
	else
	{
		out_addr(&clientaddr);
		printf("client send info: %s\n",buffer);
		//send data to client
		long t = time(0);
		char* ptr = ctime(&t);
		size_t size = strlen(ptr) * sizeof(char);
		if(sendto(sockfd, ptr, size, 0,(struct sockaddr*)&clientaddr, len) < 0)
		{
			perror("sendto error");		
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s #port\n",argv[0]);
		exit(1);
	}
	if(signal(SIGINT,sig_handler) == SIG_ERR)
	{
		perror("signal sigint error");
		exit(1);
	}	
	//step 1: creat socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}
	
	int ret;
	int opt = 1;
	//set socket options
	if((ret = setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR,& opt,sizeof(opt))) < 0)
	{
		perror("setsockopt error");
		exit(1);
	}
	
	//step 2: bind socket and address
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	
	//step 3
	while(1)
	{
		do_service();
	}
	close(sockfd);
	return 0;
}










































