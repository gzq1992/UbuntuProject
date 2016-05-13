#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<signal.h>
#include<time.h>
#include<errno.h>
#include"msg.h"
int sockfd;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("server close\n");
		close(sockfd);
		exit(1);
	}
	
	if(signo == SIGCHLD)
	{
		printf("child process deaded...\n");
		wait(0);
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

void do_service(int fd)
{
	char buff[512];
	while(1)
	{
		memset(buff, 0, sizeof(buff));
		printf("start read and write...\n");
		size_t size;
		if((size = read_msg(fd, buff, sizeof(buff))) < 0)
		{
			perror("protocol error");
			exit(-1);
		}
		else if(size == 0)
		{
			break;
		}
		else
		{
			printf("%s\n",buff);
			if(write_msg(fd, buff, sizeof(buff)) < 0)
			{
				if(errno == EPIPE)
					break;
				perror("protocol error");
			}
		}
	}
}



int main(int argc,char* argv[])
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
	
	if(signal(SIGCHLD,sig_handler) == SIG_ERR)
	{
		perror("signal sigint error");
		exit(1);
	}
	
	//step 1: creat socket
	//socket is a struct and created in kernel
	//AF_INET: IPV4
	//SOCK_STREAM: tcp protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}
	
	//step 2: bind socket and address (include ip and port)
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	//fill type of ip, port,and internet address
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[1]));	//port
	//serveraddr.sin_addr.s_addr = "192.168.75.129";
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	
	//step 3: listen
	// notify system to receive connection request from client
	//the connection request from client is put in a queue
	//second parameter: the length of the queue
	if(listen(sockfd, 10) < 0)
	{
		perror("listen error");
		exit(1);
	}
	
	//step 4: call accept function to acquire a connection from the queue
	//and return a new socket descriptor
	//if there is no client connection, it will block until acquiring a connection from client
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	
	while(1)
	{
		int fd = accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);
		if(fd < 0)
		{
			perror("accept error");
			exit(1);
		}
		
		//step 5: fork to call I/O function to communicate with client	
		pid_t pid = fork();
		if(pid < 0)
		{
			continue;		
		}
		else if(pid == 0)
		{
			//child process
			out_addr(&clientaddr);
			do_service(fd);
			//step 6: close socket service
			close(fd);
			break;
		}
		else
		{
			//parent process
			close(fd);
		}
	}	
	return 0;
}	

































	
