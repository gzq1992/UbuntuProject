#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//One thread is responsible for calculating result. 
//The other is responsible for getting result.

typedef struct{
	int res;
	int is_wait;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
}Result;



void* set_fn(void* arg)
{
	int i = 1, sum = 0;
	for(; i <=100; i++)
		sum += i;
		
	Result* r = (Result*)arg;
	r->res = sum;
	
	pthread_mutex_lock(&r->mutex);
	//Judge the pthread for getting results is ready;
	while(!r->is_wait)
	{
		pthread_mutex_unlock(&r->mutex);
		
		usleep(100);
		
		pthread_mutex_lock(&r->mutex);
	}
	
	pthread_mutex_unlock(&r->mutex);
	
	pthread_cond_broadcast(&r->cond);
	
	
	return (void*)0;
}

void* get_fn(void* arg)
{
	Result* r = (Result*)arg;
	
	pthread_mutex_lock(&r->mutex);
	//Pthread for getting results is ready.
	r->is_wait = 1;
	pthread_cond_wait(&r->cond,&r->mutex);	
	
	pthread_mutex_unlock(&r->mutex);
	
	int res = r->res;
	printf("0x%lx get sum is %d\n",pthread_self(),res);
	
	return (void*)0;
}



int main()
{
	int err;
	pthread_t cal,get;
	
	Result r;
	r.is_wait = 0;
	pthread_cond_init(&r.cond,NULL);
	pthread_mutex_init(&r.mutex,NULL);
	
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
	
	pthread_cond_destroy(&r.cond);
	pthread_mutex_destroy(&r.mutex);
	
	return 0;
}












