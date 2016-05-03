#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
#include <pthread.h>
typedef struct
{
	int code;
	double balance;
	
	pthread_mutex_t mutex;
}Account;

extern Account* create_account(int code,double balance);
extern void destroy_account(Account* a);
extern double withdraw(Account* a,double amt);
extern double deposit(Account* a,double amt);
extern double get_balance(Account* a);

#endif
