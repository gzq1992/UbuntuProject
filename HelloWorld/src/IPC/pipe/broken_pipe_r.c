#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
//Incomplete pipe
int main()
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(0);
	}
	 
	 pid_t pid;
	 if((pid = fork()) < 0)
	 {
	 	perror("fork error");
	 	exit(1);
	 }
	 else if(pid > 0)
	 {
	 	//Parent process read data from incomplete pipe(write port is closed).
	 	sleep(5);
	 	close(fd[1]);
	 	while(1)
	 	{
	 		char c;
	 		if(read(fd[0],&c,1) == 0)
	 		{
	 			printf("\nwrite-end of pipe closed\n");
	 			break;
	 		}
	 		else
	 		{
	 			printf("%c",c);
	 		}
	 	}
	 	close(fd[0]);
	 	wait(0);
	 }
	 else
	 {
	 	//Child process response for writing data to pipe
	 	close(fd[0]);
	 	char* s = "1234";
	 	write(fd[1],s,sizeof(s));
	 	close(fd[1]); 	
	 }
	 
	return 0;
}


