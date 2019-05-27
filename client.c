#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>


void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd,portno,n;
	char buffer[256];
	struct sockaddr_in serv_addr;

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
	server=gethostbyname(argv[1]);
	if(server==NULL)
	{
		fprintf(stderr,"Error,no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	inet_pton(AF_INET,argv[1],&(servaddr.sin_addr));
	serv_addr.sin_port=htons(portno);
	if (connect(sockfd,&serv_addr,sizeof(serv_addr))<0)
	{
		error(" error on connecting");
	}
    while(1)
       {
	printf("Enter the message:");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	
	n=write(sockfd,buffer,strlen(buffer));
	if(n<0)
	{
		error("error writing to socket");
	}
	bzero(buffer,256);
	
	n=read(sockfd,buffer,255);
	if(n<0)
	{
		error("error reading from socket");
	}
  printf("%s\n",buffer);
   }
return 0;
}
