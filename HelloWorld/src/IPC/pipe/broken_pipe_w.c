#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<errno.h>
#include<signal.h>

//Incomplete pipe
void sig_handler(int signo)
{
	if(signo == SIGPIPE)
	{
		printf("SIGPIPE occured\n");
	}
}

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
	 	//Parent process responses for writing data to pipe.
	 	sleep(5);
	 	close(fd[0]);
	 	if(signal(SIGPIPE,sig_handler) == SIG_ERR)
	 	{
	 		perror("signal sigpipe error");
	 		exit(1);
	 	}
	 	char* s = "1234";
	 	if(write(fd[1],s,sizeof(s)) != sizeof(s))
		{
			fprintf(stderr,"%s, %s\n",strerror(errno),(errno == EPIPE)?"EPIPE":",unknown");
		}
		close(fd[1]);
		wait(0);
	 }
	 else
	 {
	 	close(fd[0]);
	 	close(fd[1]);
	 }
	return 0;
}
