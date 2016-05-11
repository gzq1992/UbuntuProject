#ifndef _PV_H_
#define _PV_H_

//initial semnums semaphore as value
extern int I(int semnums, int value);

//do P(value) operation on semnum in semid
extern void P(int semid, int semnum, int value);

//do V(value) operation on semnum in semid
extern void V(int semid, int semnum, int value);

//destroy semid
extern void D(int semid);

#endif
