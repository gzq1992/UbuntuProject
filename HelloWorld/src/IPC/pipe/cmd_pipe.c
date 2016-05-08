#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

char* cmd1[3] = {"/bin/cat","/etc/passwd",NULL};
//char* cmd2[3] = {"/bin/grep","root",NULL};
char* cmd2[3] = {"wc","-l",NULL};



int main()
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}
	
	int i = 0;
	pid_t pid;
	for(; i < 2; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("pipe error");
			exit(1);
		}
		else if(pid == 0)
		{
			//child process
			if(i == 0)
			{
				//first child process,responsible for writing data
				close(fd[0]);
				
				if(dup2(fd[1],STDOUT_FILENO) != STDOUT_FILENO)
				{
					perror("dup2 error");
				}
				close(fd[1]);				
				
				if(execvp(cmd1[0],cmd1) < 0)
				{
					perror("execvp error");
					exit(1);
				}
				break;
			}
			if(i == 1)
			{
				//second child process,responsible for writing data
				close(fd[1]);
				if(dup2(fd[0],STDIN_FILENO) != STDIN_FILENO)
				{
					perror("dup2 error");
				}
				close(fd[0]);
								
				if(execvp(cmd2[0],cmd2) < 0)
				{
					perror("execvp error");
					exit(1);
				}
				break;
			}
		}
		else
		{
			//parent process
			if(i == 1)
			{
				//Parent process will not recycle before child processes are all estiblished.
				close(fd[0]);
				close(fd[1]);
				wait(0);
				wait(0);
			}
		}
	}	
	return 0;
}

