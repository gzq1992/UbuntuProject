#include "tell.h"
#include <stdio.h>
//#include<sys/shm.h>
#include <stdlib.h>
#include <string.h>

static int fd[2];

void init()
{
	if(pipe(fd) < 0)
	{
		perror("pipe error");
	}
}

//Read and write of pipe is default blocked.
void wait_pipe()
{
	char c;
	if(read(fd[0],&c,1) < 0)
	{
		perror("wait pipe error");
	}
}

void notify_pipe()
{
	char c = 'c';
	if(write(fd[1],&c,1) != 1)
	{
		perror("notify pipe error");
	}
}

void destroy_pipe()
{
	close(fd[0]);
	close(fd[1]);
}

