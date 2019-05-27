#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pthread.h>

pthread_t sendt,rect;

       int sockfd,portno,n;
	char buffer[256];
	struct sockaddr_in serv_addr;

void error(char *msg)
{
	perror(msg);
	exit(0);
}

void sender()
{
	while(1)
     {
       	bzero(buffer,256);
	fgets(buffer,255,stdin);
	int j=strncmp("bye",buffer,3);
	if(j==0)
	break;
	
	n=write(sockfd,buffer,strlen(buffer));
	if(n<0)
	{
		error("error writing to socket");
	}
	bzero(buffer,256);
      }
     
}

void receiver()
{
   
	while(1)
    {
       bzero(buffer,256);
	
	n=read(sockfd,buffer,255);
	if(n<0)
	{
		error("error reading from socket");
	}
       printf("Server:%s\n",buffer);
	int j=strncmp("bye",buffer,3);
	if(j==0)
	break;
    }
}


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage %s hostname port\n",argv[0]);
		exit(0);
	}
	
	portno=atoi(argv[2]);
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	inet_pton(AF_INET,argv[1],&(servaddr.sin_addr));
	serv_addr.sin_port=htons(portno);
	if (connect(sockfd,&serv_addr,sizeof(serv_addr))<0)
	{
		error(" error on connecting");
	}
   
   pthread_create(&sendt,NULL,sender,0);
  pthread_create(&rect,NULL,receiver,0);

  pthread_join(sendt,NULL);
  pthread_join(rect,NULL);

return 0;
}

