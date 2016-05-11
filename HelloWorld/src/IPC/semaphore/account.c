#include "account.h"
#include <assert.h>
#include <string.h>
#include <time.h>
#include "pv.h"

double withdraw(Account* a,double amt)
{
	assert(a != NULL);
	
	P(a->semid, 0, 1);
	
	if(amt < 0 || amt > a->balance)
	{
		V(a->semid, 0, 1);
		return 0.0;
	}
	double balance = a->balance;
	sleep(1);
	balance -= amt;
	a->balance = balance;
	
	V(a->semid, 0, 1);
	
	return amt;
}

double deposit(Account* a,double amt)
{
	assert(a != NULL);
	
	P(a->semid, 0, 1);
	
	if(amt < 0)
	{
		V(a->semid, 0, 1);
		return 0.0;
	}
	double balance = a->balance;
	sleep(1);
	balance += amt;
	a->balance = balance;
	
	V(a->semid, 0, 1);
	
	return amt;
}

double get_balance(Account* a)
{
	assert(a != NULL);
	
	P(a->semid, 0, 1);
	
	double balance = a->balance;
	
	V(a->semid, 0, 1);
	
	return balance;
}

