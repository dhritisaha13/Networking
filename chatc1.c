#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

int portno;

void *sender(void *ptr)
{
	int sfd=*(int *)ptr;
	char buf[1000];
	while(1)
	{
		for (int y = 0; y < sizeof(buf); y++)
            	buf[y]='\0';
		
 		gets(buf);
		if(strcmp(buf,"end")==0)
 	 	{
 	 		close(sfd);
 	 		free(ptr);
 	 		printf("Client is Disconnected\n");
 	 		exit(1);
 	 	}
 	 	send(sfd,buf,strlen(buf),0);
 		for (int y = 0; y < sizeof(buf); y++)
            	buf[y]='\0';
 	}
}

void *receiver(void *p)
{
	int sfd=*(int *)p;
	char buf[1000];
	for (int y = 0; y < sizeof(buf); y++)
            buf[y]='\0';
	while((recv(sfd,buf,1000,0)))
 	{
 		printf("Received Message is: %s\n",buf);
 		for (int y = 0; y < sizeof(buf); y++)
            	buf[y]='\0';
 	}
 	if((recv(sfd,buf,1000,0))==0)
 	{
 		printf("Server is Disconnected\n");
 		exit(1);
 	}

}


int main(int argc, char *argv[])
{
	portno = atoi(argv[1]);
	int sockfd;
	int *z;
	pthread_t tid1,tid2;
	struct sockaddr_in servaddr;

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&servaddr,'\0',sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(portno);
	inet_pton(AF_INET,argv[2],&(servaddr.sin_addr));
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
 	
 	z=malloc(1);
 	*z=sockfd;
 	pthread_create(&tid1,NULL,sender,(void *)z);
 	pthread_create(&tid2,NULL,receiver,(void *)z);
 	pthread_join(tid1,NULL);
 	pthread_join(tid2,NULL);
 	return 0;
}


