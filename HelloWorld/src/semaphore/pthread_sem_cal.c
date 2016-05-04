#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
//One thread is responsible for calculating result. 
//Some threads are responsible for getting result.

typedef struct{
	int res;
	sem_t sem;
}Result;



void* set_fn(void* arg)
{
	int i = 1, sum = 0;
	for(; i <=100; i++)
		sum += i;
		
	Result* r = (Result*)arg;
	sem_post(&r->sem);
	r->res = sum;
	return (void*)0;
}

void* get_fn(void* arg)
{
	Result* r = (Result*)arg;
	sem_wait(&r->sem);
	int res = r->res;
	printf("0x%lx get sum is %d\n",pthread_self(),res);
	
	return (void*)0;
}



int main()
{
	int err;
	pthread_t cal,get;
	
	Result r;
	sem_init(&r.sem,0,0);
	
	if((err = pthread_create(&get,NULL,get_fn,(void*)&r)) != 0)
	{
		perror("pthread create error");
	}
	if((err = pthread_create(&cal,NULL,set_fn,(void*)&r)) != 0)
	{
		perror("pthread create error");
	}

	pthread_join(cal,NULL);
	pthread_join(get,NULL);

	sem_destroy(&r.sem);
	
	return 0;
}












