#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("usage:%s fifo\n",argv[0]);
		exit(1);
	}
	printf("open fifo write...\n");
	int fd = open(argv[1],O_WRONLY);
	
	if(fd < 0)
	{
		perror("open error");
		exit(1);
	}
	else
	{
		printf("open file success:%d\n",fd);
	}
	
	char* s = "1234567890";
	size_t size = strlen(s);
	if(write(fd,s,size) != size)
	{
		perror("write error");
	}
	close(fd);
	return 0;
}
