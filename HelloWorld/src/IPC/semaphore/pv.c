#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "pv.h"

union semun{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
};


//initial semnums semaphore as value
int I(int semnums, int value)
{
	//creat semid
	int semid;
	semid = semget(IPC_PRIVATE, semnums, IPC_CREAT | IPC_EXCL | 0777);
	if(semid < 0)
	{
		return -1;
	}
	union semun un;
	unsigned short* array = (unsigned short*)calloc(semnums,sizeof(unsigned short));
	int i;
	for(i = 0; i < semnums; i++)
	{
		array[i] = value;
	}
	un.array = array;
	if(semctl(semid, 0, SETALL, un) < 0)
	{
		perror("semctl error");
		return -1;
	}
	free(array);
	return semid;
}

//do P(value) operation on semnum in semid
void P(int semid, int semnum, int value)
{
	assert(value >= 0);
	//define sembuf struct array to place some struct variables which represent the semaphore and P or V operation
	struct sembuf ops[] = {{semnum, -value, SEM_UNDO}};
	
	if(semop(semid, ops,sizeof(ops)/sizeof(struct sembuf)) < 0)
	{
		perror("semop error");
	}
}

//do V(value) operation on semnum in semid
void V(int semid, int semnum, int value)
{
	assert(value >= 0);
	//define sembuf struct array to place some struct variables which represent the semaphore and P or V operation
	struct sembuf ops[] = {{semnum, value, SEM_UNDO}};
	
	if(semop(semid, ops,sizeof(ops)/sizeof(struct sembuf)) < 0)
	{
		perror("semop error");
	}
}

//destroy semid
void D(int semid)
{
	if(semctl(semid, 0, IPC_RMID, NULL) < 0)
	{
		perror("semctl error");
	}
}











































