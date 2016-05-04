#include "account.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <time.h>

Account* create_account(int code,double balance)
{
	Account* a = (Account*)malloc(sizeof(Account));
	assert(a != NULL);
	a->code = code;
	a->balance = balance;
	//pthread_mutex_init(&a->mutex,NULL);
	//pthread_rwlock_init(&a->rwlock,NULL);
	sem_init(&a->sem,0,1);
	return a;
}
void destroy_account(Account* a)
{
	assert(a != NULL);
	
	//pthread_mutex_destroy(&a->mutex);
	//pthread_rwlock_destroy(&a->rwlock);
	sem_destroy(&a->sem);
	free(a);
}

double withdraw(Account* a,double amt)
{
	assert(a != NULL);
	
	//pthread_mutex_lock(&a->mutex);
	//pthread_rwlock_wrlock(&a->rwlock);	
	sem_wait(&a->sem);
	if(amt < 0 || amt > a->balance)
	{
		//pthread_mutex_unlock(&a->mutex);
		//pthread_rwlock_unlock(&a->rwlock);
		sem_post(&a->sem);
		return 0.0;
	}
	double balance = a->balance;
	sleep(1);
	balance -= amt;
	a->balance = balance;
	
	//pthread_mutex_unlock(&a->mutex);
	//pthread_rwlock_unlock(&a->rwlock);
	sem_post(&a->sem);
	return amt;
}

double deposit(Account* a,double amt)
{
	assert(a != NULL);
	
	//pthread_mutex_lock(&a->mutex);
	//pthread_rwlock_wrlock(&a->rwlock);
	sem_wait(&a->sem);
	
	if(amt < 0)
	{
		//pthread_mutex_unlock(&a->mutex);
		//pthread_rwlock_unlock(&a->rwlock);
		sem_post(&a->sem);
		return 0.0;
	}
	double balance = a->balance;
	sleep(1);
	balance += amt;
	a->balance = balance;
	
	//pthread_mutex_unlock(&a->mutex);
	//pthread_rwlock_unlock(&a->rwlock);
	sem_post(&a->sem);
	return amt;
}

double get_balance(Account* a)
{
	assert(a != NULL);
	
	//pthread_mutex_lock(&a->mutex);
	//pthread_rwlock_rdlock(&a->rwlock);
	sem_wait(&a->sem);
	double balance = a->balance;
	
	//pthread_mutex_unlock(&a->mutex);
	//pthread_rwlock_unlock(&a->rwlock);
	sem_post(&a->sem);
	return balance;
}

