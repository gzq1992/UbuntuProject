#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include "tell.h"

int main()
{
	int shmid;
	//creat shared memory
	if((shmid = shmget(IPC_PRIVATE,1024,IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget error");
		exit(0);
	}
	pid_t pid;
	init();
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//father process
		//map shared memory to father process
		int* pi = (int*)shmat(shmid,0,0);
		if(pi == (int*)-1)
		{
			perror("shmat error");
			exit(1);
		}
		//write data to shared memory
		*pi = 100;
		*(pi + 1) = 200;
		//release map of shared memory after operation
		shmdt(pi);
		//notify child process to read data
		notify_pipe();
		destroy_pipe();
		wait(0);
	}
	else
	{
		//child process
		//block child process, wait for parent process writint data to shared memory 
		wait_pipe();
		//read data from shared memory
		//map shared memory to child process
		int* pi = (int*)shmat(shmid,0,0);
		if(pi == (int*)-1)
		{
			perror("shmat error");
			exit(1);
		}
		printf("start: %d, end: %d\n", *pi, *(pi + 1));		
		shmdt(pi);
		
		//delete shared memory
		shmctl(shmid, IPC_RMID,NULL);	
			
		destroy_pipe();
	}
	
	return 0;
}










































