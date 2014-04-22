/* Copyright (C) 2014 Denis V. Meltsaykin */

#include <stdio.h>
#include <stdlib.h>

#include <sys/param.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

#include <strings.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>


#define log(x, arg...)	fprintf(stderr, ## arg)
#define panic(x)	fprintf(stderr, "PANIC: %s", x), exit(1)
#define min(a, b) ((a) < (b) ? (a) : (b) )
#include <net/radix.h>



#include "test_routes.h"

#define offsetof(t,m) (int)((&((t *)0L)->m))
#define OFF_LEN_INET	(8 * offsetof(struct sockaddr_in, sin_addr))
#define KEY_LEN(v)	*((uint8_t *)&(v))
#define KEY_LEN_INET	(offsetof(struct sockaddr_in, sin_addr) + sizeof(in_addr_t))

struct entry {
	struct radix_node ent[2];
	struct sockaddr_in addr, mask;
	uint32_t value;
};

int main () 

{

	struct rusage start, end;
	
	static struct radix_node_head *RHN=NULL;
	struct entry *returned1;
	struct radix_node *returned;
	
	rn_inithead ((void **)&RHN, OFF_LEN_INET);
    
	struct sockaddr *route;
	struct sockaddr *mask;
	struct sockaddr_in *pmask,*proute;
	struct entry *ent;
	void *ent_ptr;

	char ip[16];
	
	int i=0;
	int j=0;
	int k=0;
	int xx=0;
	
	sranddev();
	
	for (k=5; k < 255; k+=10)
	for (i=1; i < 255; i+=2) 
	for (j=1; j < 255; j+=3)
	{
		
		if ((rand() % 10) > 3) {
		} else {
		ent = malloc (sizeof(struct entry));
		KEY_LEN(ent->addr) = KEY_LEN_INET;
		KEY_LEN(ent->mask) = KEY_LEN_INET;
		
		sprintf(ip, "10.%d.%d.%d",k, i,j);
		inet_aton(ip,&ent->addr.sin_addr);
		
		sprintf(ip, "255.255.255.%d",rand()%254 + 1);
		
		inet_aton(ip, &ent->mask.sin_addr);
		
		route = (struct sockaddr *) &ent->addr;
		mask = (struct sockaddr *) &ent->mask;
		ent->value=(++xx);
		ent_ptr = ent;
		returned=RHN->rnh_addaddr(route, mask, RHN, ent_ptr);
		if (NULL == returned) printf("RHN_addaddr (%s) failed\n", ip);
		};
	};
	
	route = malloc (sizeof (struct sockaddr));
	mask = malloc (sizeof (struct sockaddr));
	KEY_LEN(route) = KEY_LEN_INET;
	pmask = (struct sockaddr_in *) mask;
	proute = (struct sockaddr_in *) route;
	
	inet_aton("255.255.255.255", &pmask->sin_addr);
	inet_aton("10.55.201.91", &proute->sin_addr);
	
	getrusage(RUSAGE_SELF, &start);
	returned1 = (struct entry *) RHN->rnh_lookup(route, NULL, RHN);
	getrusage(RUSAGE_SELF, &end);

	if (returned1 != NULL) printf ("SUCCESS! VALUE:%d\n", returned1->value);
	printf ("TIME: %i.%ld : %i.%ld usec\n",\
			start.ru_utime.tv_sec, start.ru_utime.tv_usec,\
			end.ru_utime.tv_sec, end.ru_utime.tv_usec);
	printf ("SIZE: %d\t COUNT:%d \n",sizeof(struct entry)*xx, xx);
    return 0;

}
