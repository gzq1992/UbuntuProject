#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>



int main()
{
	int fda[2],fdb[2];
	if((pipe(fda) < 0) || (pipe(fdb) < 0))
	{
		perror("pipe error");
		exit(0);
	}
	
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		//child process
		//(1)Child process reponses for reading parameter x and y from pipe a from parent process
		//(2)Call add function for addition by exec function
		//(3)Write addition results to pipe b.  
		close(fda[1]);
		close(fdb[0]);
		
		if(dup2(fda[0],STDIN_FILENO) != STDIN_FILENO)
		{
			perror("dup2 error");
		}
		
		if(dup2(fdb[1],STDOUT_FILENO) != STDOUT_FILENO)
		{
			perror("dup2 error");
		}
		
		if(execlp("/home/gzq/codes/IPC/add","/home/gzq/codes/IPC/add",NULL) < 0)
		{
			perror("execlp error");
			exit(1);
		}
	}
	else
	{
		//parent process
		//(1)Read parameter x and y from STDIN
		//(2)Write x and y to pipe a
		//(3)Read addition result from pipe b and print it
		close(fda[0]);
		close(fdb[1]);
		int x,y;
		printf("please input x and y: ");
		//step 1
		scanf("%d %d",&x,&y);
		//step 2
		if(write(fda[1],&x,sizeof(int)) != sizeof(int))
		{
			perror("wirte error");
			exit(1);
		}
		if(write(fda[1],&y,sizeof(int)) != sizeof(int))
		{
			perror("wirte error");
			exit(1);
		}	
		//step 3
		int result = 0;
		if(read(fdb[0],&result,sizeof(int)) < 0)
		{
			perror("read error");
			exit(1);
		}
		else
		{
			printf("add result is %d\n",result);
		}
		close(fda[1]);
		close(fdb[0]);
		wait(0);
	}
	

	return 0;
}
































