#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void out_addr(struct hostent* h)
{
	printf("hostname: %s\n",h->h_name);
	printf("addrtype: %s\n",h->h_addrtype == AF_INET ? "IPV4" : "IPV6");
	char ip[16];
	memset(ip, 0, sizeof(ip));
	inet_ntop(h->h_addrtype, h->h_addr_list[0], ip, sizeof(ip));
	printf("ip address: %s\n", ip);
	int i = 0;
	while(h->h_aliases[i] != NULL)
	{
		printf("aliase: %s\n", h->h_aliases[i]);
		i++;
	}
}


int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s host\n",argv[0]);
		exit(1);
	}
	struct hostent* h;
	
	h= gethostbyname(argv[1]);
	if(h != NULL)
	{
		out_addr(h);
	}
	else
	{
		printf("no %s exist\n", argv[1]);
	}
	endhostent();
	return 0;
}































