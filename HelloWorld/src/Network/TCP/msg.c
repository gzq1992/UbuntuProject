#include "msg.h"
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>

static unsigned char msg_check(MSG* message)
{
	unsigned char s = 0;
	int i = 0;
	for(i = 0; i < sizeof(message->head); i++)
	{
		s += message->head[i];
	}
	for(i = 0; i < sizeof(message->buff); i++)
	{
		s += message->buff[i];
	}
	return s;
}

//send a message based on user-defined protocol
int write_msg(int sockfd, char* buff, size_t len)
{
	MSG message;
	memset(&message, 0, sizeof(message));
	strcpy(message.head,"iotek2012");
	memcpy(message.buff, buff, len);
	message.checknum = msg_check(&message);
	if(write(sockfd,&message,sizeof(message)) != sizeof(message))
	{
		return -1;
	}
	return 0;
}

//read
int read_msg(int sockfd, char* buff, size_t len)
{
	MSG message;
	memset(&message, 0, sizeof(message));
	size_t size;
	if((size = read(sockfd, &message, sizeof(message))) < 0)
	{
		return -1;
	}
	else if(size == 0)
	{
		return 0;
	}
	unsigned char s = msg_check(&message);
	if((s == message.checknum) && (!strcmp("iotek2012",message.head)))
	{
		memcpy(buff, message.buff, len);
		return sizeof(message);
	}
	return -1;
}
