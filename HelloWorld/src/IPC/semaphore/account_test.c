#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include "account.h"
#include "pv.h"

int main()
{
	int shmid;
	//creat shared memory
	if((shmid = shmget(IPC_PRIVATE,sizeof(Account),IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget error");
		exit(0);
	}
	
	Account* a = (Account*)shmat(shmid,0,0);
	if(a == (Account*)-1)
	{
		perror("shmat error");
		exit(1);
	}		
	a->code = 100001;
	a->balance = 10000;
	
	//creat semid and initial
	
	a-> semid = I(1,1);
	if(a->semid < 0)
	{
		perror("I(1,1) init error");
		exit(0);
	}
	
	
	printf("balance: %f\n", a->balance);
	
	pid_t pid;
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		double amt = withdraw(a,10000);
		printf("pid %d withdraw %f from code %d\n",getpid(),amt,a->code);
		wait(0);
		printf("balance: %f\n",a->balance);
		D(a->semid);
		shmdt(a);
		//destroy shared memory
		shmctl(shmid, IPC_RMID, NULL);
	}
	else
	{
		double amt = withdraw(a,10000);
		printf("pid %d withdraw %f from code %d\n",getpid(),amt,a->code);
		shmdt(a);
	}
	
	return 0;
}















































