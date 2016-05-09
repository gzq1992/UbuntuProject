#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
	int i = 0;
	for(;i < 2;i++)
	{
		fork();
		printf("%d ", i);
	}
	return 0;
}
