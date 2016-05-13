#ifndef _MSG_H_
#define _MSG_H_H
#include <sys/types.h>
typedef struct{
	//protocol head
	char head[10];
	unsigned char checknum; //for check
	
	//protocol body
	char buff[512]; //data
}MSG;

//send a message based on user-defined protocol
extern int write_msg(int sockfd, char* buff, size_t len);

//read
extern int read_msg(int sockfd, char* buff, size_t len);

#endif
