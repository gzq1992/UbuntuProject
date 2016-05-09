#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
	int x,y;
	if(read(STDIN_FILENO,&x,sizeof(int)) < 0)
	{
		perror("read error");
	}
	if(read(STDIN_FILENO,&y,sizeof(int)) < 0)
	{
		perror("read error");
	}
	int result = x + y;
	if(write(STDOUT_FILENO,&result,sizeof(int)) != sizeof(int))
	{
		perror("write error");
	}
	//exit(0);
	return 0;
}
