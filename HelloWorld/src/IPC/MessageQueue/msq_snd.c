#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/msg.h>

typedef struct{
	long type;	//message type
	int start;  //message(include start and end)
	int end;
}MSG;


//send message to message queue
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s key\n",argv[0]);
		exit(1);
	}
	key_t key = atoi(argv[1]);
	//key_t key = IPC_PRIVATE;
	//key_t key = ftok(argv[1],0);	//ftok ensures the key is unique
	printf("key: %d\n",key);
	//create message queue
	int msq_id;
	if((msq_id = msgget(key, IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("msgget error");
	}
	
	printf("msq id: %d\n",msq_id);
	
	//define message to send
	MSG m1 = {4,4,400};
	MSG m2 = {2,2,200};
	MSG m3 = {1,1,100};
	MSG m4 = {6,6,600};
	MSG m5 = {6,60,6000};
	
	//send message to message queue
	if(msgsnd(msq_id,&m1,sizeof(MSG) - sizeof(long),IPC_NOWAIT) < 0)
	{
		perror("msgsnd error");
	}
	
	if(msgsnd(msq_id,&m2,sizeof(MSG) - sizeof(long),IPC_NOWAIT) < 0)
	{
		perror("msgsnd error");
	}
	
	if(msgsnd(msq_id,&m3,sizeof(MSG) - sizeof(long),IPC_NOWAIT) < 0)
	{
		perror("msgsnd error");
	}
	
	if(msgsnd(msq_id,&m4,sizeof(MSG) - sizeof(long),IPC_NOWAIT) < 0)
	{
		perror("msgsnd error");
	}
	
	if(msgsnd(msq_id,&m5,sizeof(MSG) - sizeof(long),IPC_NOWAIT) < 0)
	{
		perror("msgsnd error");
	}
	
	//get total number of messages in the message queue after sending message
	
	struct msqid_ds ds;
	if(msgctl(msq_id,IPC_STAT,&ds) < 0)
	{
		perror("msgctl error");
	}
	
	printf("msg total: %ld\n",ds.msg_qnum);
	
	
	return 0;
}
