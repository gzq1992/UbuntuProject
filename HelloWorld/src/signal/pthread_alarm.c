#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
void sig_handler(int signo)
{
	printf("pthread id in the sig_handler: %lx\n",pthread_self());
	if(signo == SIGALRM)
	{
		printf("timeout...\n");
	}
	alarm(2);
}

void* th_fn(void* arg)
{
	if(signal(SIGALRM, sig_handler) == SIG_ERR)
	{
		perror("signal sigalrm error");
	}
	alarm(2);
	int i = 1;
	for(;i <= 100; i++)
	{
		printf("(%lx) i: %d\n",pthread_self(),i);
		sleep(1);
	}
	return (void*)0;
}

//terminate thread th_fn
void* th_fn2(void* arg)
{
	pthread_t th1 = (pthread_t)arg;
	int i = 1;
	for(;i <= 15; i++)
	{
		if(i == 5)
		{
			pthread_cancel(th1);
			alarm(0);
		}
		printf("(%lx) i: %d\n",pthread_self(),i);
		sleep(1);
	}
	return (void*)0;
}


int main()
{
	pthread_t th,th2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int err;
	
	if((err = pthread_create(&th,&attr,th_fn,(void*)0)) != 0)
	{
		perror("pthread create error");
	}
	
	if((err = pthread_create(&th2,&attr,th_fn2,(void*)th)) != 0)
	{
		perror("pthread create error");
	}
	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGALRM);
	//Mask SIGALRM for main control thread.
	pthread_sigmask(SIG_SETMASK,&set,NULL);
	
	
	while(1)
	{
		printf("control thread(%lx) is running.\n",pthread_self());
		sleep(10);
	}
	printf("control thread over\n");
	
	pthread_attr_destroy(&attr);
	
	
	
	return 0;
}

/*
#include<stdio.h>
#include<signal.h>
#include<time.h>

void handler()
{
	printf("hello\n");
	signal(SIGALRM,handler);
	alarm(5);
}

int main()
{
	int i;
	handler();
	for(i = 1; i < 21;i++)
	{
		printf("sleep %d ...\n",i);
		sleep(1);
	}
	return 0;
}
*/

