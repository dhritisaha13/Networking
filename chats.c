#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<pthread.h>

pthread_t sendt,rect;


     int sockfd,newsockfd,portno,clilen,n;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;


void error(char *msg)
{
	perror(msg);
	exit(1);
}

void receiver()
{
    
	while(1)
    {
	n=read(newsockfd,buffer,255);
	if(n<0)
	{
		error("error reading from socket");
	}
	printf("Client:%s\n",buffer);
	int j=strncmp("bye",buffer,3);
	if(j==0)
	break;
	
     } 
}

void sender()
{
  
	while(1)
      {
	bzero(buffer,256);
	fgets(buffer,255,stdin);
      	int j=strncmp("bye",buffer,3);
       	n=write(newsockfd,buffer,100);
	if(j==0)
	break;
	bzero(buffer,256);
	if(n<0)
	{
		error("error writing to socket");
	
	}
     }
}

int main(int argc, char *argv[])
{
	
	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Opening socket");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno=atoi(argv[1]);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portno);

	if (bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
		error(" error on binding");
	}

	listen(sockfd,5);
	clilen=sizeof(cli_addr);

	newsockfd=accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd<0)
	{
		error("error on accept");
	}
	bzero(buffer,256);
  

  pthread_create(&sendt,NULL,sender,0);
  pthread_create(&rect,NULL,receiver,0);

  pthread_join(sendt,NULL);
  pthread_join(rect,NULL);

return 0;

}


