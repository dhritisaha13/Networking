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
int i=0;
int arrayfd[10];

void *sender(void *ptr)
{
	int sock=*(int *)ptr;
	char buf[1000];
	char a[10];
	for (int y = 0; y < sizeof(a); y++)
            a[y]='\0';

    	for (int y = 0; y < sizeof(buf); y++)
            buf[y]='\0';


	while((recv(sock,buf,1000,0)))
	{
 	
 		printf("Message from Client: %s\n",buf);

 		while((recv(sock,a,10,0)))
 		{
 		printf("Sending to Client %s\n",a);
 			break;
 		}
 		int i=atoi(a);
 		send(arrayfd[i],buf,strlen(buf),0);
 		for (int y = 0; y < sizeof(a); y++)
            	a[y]='\0';

    		for (int y = 0; y < sizeof(buf); y++)
           	buf[y]='\0';

 	}
 	free(ptr);
 	return 0;
}



int main(int argc, char *argv[])
{
	portno = atoi(argv[1]);
	int sockfd;
	int *z;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	pthread_t tid1;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&servaddr,'\0',sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(portno);
	inet_pton(AF_INET,argv[2],&(servaddr.sin_addr));

	bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
 	

 	listen(sockfd,10);

 	clilen=sizeof(cliaddr);
 	while((arrayfd[i]=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen)))
 	{
 		z=malloc(1);
 		*z=arrayfd[i];
 		i++;
 		
 		pthread_create(&tid1,NULL,sender,(void *)z);
 	}
 	return 0;
 }

