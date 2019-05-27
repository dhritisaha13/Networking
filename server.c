#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<pthread.h>
#include<sys/time.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"Port number not provided!! Program terminated!!\n");
		exit(1);
	}

	int sockfd, newsockfd, *new_sock, portno, n;
	char buffer[1307];

	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen; 

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0)
	{
		error("Error Opening Socket!!\n");
	}

	bzero((char*)&serv_addr,sizeof(serv_addr));	
	bzero((char*)&cli_addr,sizeof(cli_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, ((struct sockaddr*)&serv_addr), sizeof(serv_addr)) < 0)
		error("Binding Failed!!!\n");

	int len1 = sizeof(serv_addr);
	int len2 = sizeof(cli_addr);
		
	while(1)
	{
		memset(buffer,'\0',1307);
		if(n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&cli_addr,&len1 ) == -1)
		{
			error("Receive error!!!\n");
		}
		buffer[6]--;
		if(n = sendto(sockfd,buffer,n,0,(struct sockaddr*)&cli_addr,len2) == -1)
		{
			error("Sending Error!!!\n");
		}
	}
}





