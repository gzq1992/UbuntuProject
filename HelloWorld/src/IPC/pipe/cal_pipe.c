#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

/*
	Parent process deliver two data to child process by pipe.
	And child process read and output data from pipe. 
*/

int main()
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}
	
	pid_t pid;	
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//parent process
		close(fd[0]);	//Parent process write data.
		int start = 1, end = 100;
		if(write(fd[1],&start,sizeof(int)) != sizeof(int))
		{
			perror("wirte error");
			exit(1);
		}
		if(write(fd[1],&end,sizeof(int)) != sizeof(int))
		{
			perror("wirte error");
			exit(1);
		}
		close(fd[1]);
		wait(0);
	}
	else
	{
		//child process
		close(fd[1]); //Child process read data.
		int start = 0,end = 0;
		
		if(read(fd[0],&start,sizeof(int)) < 0)
		{
			perror("read error");
			exit(1);
		}
		if(read(fd[0],&end,sizeof(int)) < 0)
		{
			perror("read error");
			exit(1);
		}
		close(fd[0]);
		printf("child process read start: %d, end:%d \n",start,end);
	}
	return 0;
}






















