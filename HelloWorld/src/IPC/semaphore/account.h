#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

typedef struct
{
	int code;
	double balance;
	int semid; //bind semid to shared resources
}Account;

extern double withdraw(Account* a,double amt);

extern double deposit(Account* a,double amt);

extern double get_balance(Account* a);

#endif
