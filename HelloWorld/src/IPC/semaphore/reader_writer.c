#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/shm.h>
#include <sys/sem.h>

//shared resource
typedef struct{
	int val;
	int semid;
}Storage;

void init(Storage* s)
{
	assert(s != NULL);
	//creat semid(include 2 semaphore)
	if((s->semid = semget(IPC_PRIVATE, 2, IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("semget error");
		exit(1);
	}
	//initial semaphore
	union semun{
		int val;
		struct semid_ds* ds;
		unsigned short* array;
	};
	union semun un;
	unsigned short array[2] = {0,0};
	un.array = array;
	if(semctl(s->semid, 0, SETALL, un) < 0)
	{
		perror("semctl error");
		//exit(1);
	}
}

void destroy(Storage* s)
{
	assert(s != NULL);
	if(semctl(s->semid, 0, IPC_RMID, NULL) < 0)
	{
		perror("semctl error");
	}
}

void Write(Storage* s, int val)
{
	assert(s != NULL);
	s->val = val;
	printf("%d write %d\n", getpid(), val);
	
	struct sembuf ops_v[1] = {{0, 1, SEM_UNDO}};
	struct sembuf ops_p[1] = {{1, -1, SEM_UNDO}};
	
	//V(s1)
	if((semop(s->semid, ops_v, sizeof(ops_v)/sizeof(struct sembuf))) < 0)
	{
		perror("semop error");
	}
	
	//P(s2)
	if((semop(s->semid, ops_p, sizeof(ops_v)/sizeof(struct sembuf))) < 0)
	{
		perror("semop error");
	}
}

void Read(Storage* s)
{
	assert(s != NULL);
	struct sembuf ops_v[1] = {{1, 1, SEM_UNDO}};
	struct sembuf ops_p[1] = {{0, -1, SEM_UNDO}};
	//P(s1)
	if((semop(s->semid, ops_p, sizeof(ops_v)/sizeof(struct sembuf))) < 0)
	{
		perror("semop error");
	}
	
	//read data
	printf("%d read %d\n", getpid(), s->val);
	
	//V(s2)
	if((semop(s->semid, ops_v, sizeof(ops_v)/sizeof(struct sembuf))) < 0)
	{
		perror("semop error");
	}

}

int main()
{
	int shmid;
	if((shmid  = shmget(IPC_PRIVATE, sizeof(Storage), IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget error");
		exit(1);
	}
	
	Storage* s = (Storage*) shmat(shmid,0,0);
	if(s == (Storage*)-1)
	{
		perror("shmat error");
		exit(1);
	}
	
	init(s);
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		int i = 1;
		for(; i <= 100; i++)
		{
			Write(s, i);
		}
		wait(0);
		destroy(s);
		shmdt(s);
		shmctl(shmid, IPC_RMID, NULL);
	}
	else
	{
		int i = 1;
		for(; i <= 100; i++)
		{
			Read(s);
		}
		shmdt(s);
	}	
	return 0;
}
