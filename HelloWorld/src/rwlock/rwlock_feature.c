#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
pthread_rwlock_t rwlock;

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("-usage:%s [r|w] [r|w]\n",argv[0]);
		exit(0);
	}

	pthread_rwlock_init(&rwlock,NULL);
	if(!strcmp("r",argv[1]))
	{
		if(pthread_rwlock_rdlock(&rwlock) != 0)
		{
			printf("first read lock failure\n");
		}
		else
		{
			printf("first read lock success\n");
		}
	}
	else if(!strcmp("w",argv[1]))
	{
		if(pthread_rwlock_wrlock(&rwlock) != 0)
		{
			printf("first write lock failure\n");
		}
		else
		{
			printf("first write lock success\n");
		}
	}

	if(!strcmp("r",argv[2]))
	{
		if(pthread_rwlock_rdlock(&rwlock) != 0)
		{
			printf("second read lock failure\n");
		}
		else
		{
			printf("second read lock success\n");
		}
	}
	else if(!strcmp("w",argv[2]))
	{
		if(pthread_rwlock_wrlock(&rwlock) != 0)
		{
			printf("second write lock failure\n");
		}
		else
		{
			printf("second write lock success\n");
		}
	}

	pthread_rwlock_unlock(&rwlock);
	pthread_rwlock_unlock(&rwlock);
	
	return 0;
}
