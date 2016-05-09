#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

void clean_fun(void* arg)
{
	char *s = (char*)arg;
	printf("clean_func: %s\n",s);
}

void* th_fun(void* arg)
{
	int execute = (int)arg;
	pthread_cleanup_push(clean_fun,"the first clean func");
	pthread_cleanup_push(clean_fun,"the second clean func");
	printf("thread running %lx\n",pthread_self());
	pthread_cleanup_pop(execute);
	pthread_cleanup_pop(execute);
	return (void*)0;
}


int main()
{
	int err;
	pthread_t th1,th2;
	
	if((err = pthread_create(&th1, NULL,th_fun,(void*)1)) != 0)
	{
		perror("pthread_create error");
	}

	//pthread_join(th1,NULL);
	//printf("th1 %lx finished\n",th1);
	

	if((err = pthread_create(&th2, NULL,th_fun,(void*)1)) != 0)
	{
		perror("pthread_create error");
	}
	
	pthread_join(th1,NULL);
	printf("th1 %lx finished\n",th1);
	
	
	
	pthread_join(th2,NULL);
	printf("th2 %lx finished\n",th2);
	return 0;
}

