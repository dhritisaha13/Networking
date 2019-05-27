#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<sys/time.h>


struct timeval t;
uint32_t time;
uint32_t sendtime;

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


void Time()
{
	int n;
	if( (n = gettimeofday(&t,NULL)) == -1)
		error("Time error");
	time = (uint32_t)t.tv_sec;
}

uint32_t timeStamp()
{
	gettimeofday(&t,NULL);
	return ((uint32_t)t.tv_sec - time)*1000000 + (uint32_t)t.tv_usec;
}

void bufferfill(char* buffer,int value,int position,int bytes)
{
	for(int i=position+bytes-1;i>=position;i--)
	{
		buffer[i] = value%256;
		value /= 256; 
	}
}

void createdatagram(char* buffer,int i,int TTL,int p)
{
	sendtime = timeStamp();
	bufferfill(buffer,i,0,2);
	bufferfill(buffer,sendtime,2,4);
	bufferfill(buffer,TTL,6,1);
	
}

int main(int argc, char const *argv[])
{
	int sockfd, portno, n, length_server;
	struct sockaddr_in serv_addr;
	uint32_t rectime;

	char buffer[1307];
	if(argc<6)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	int Payload = atoi(argv[3]);
	int TTL = atoi(argv[4]);
	int numpackets = atoi(argv[5]);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		error("Error Opening Socket.\n");
	}

	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	inet_pton(AF_INET,argv[1],&(serv_addr.sin_addr));
	serv_addr.sin_port = htons(portno);
	char* file = (char*)argv[4];
	strcat(file, ".csv");
	FILE *fp;
	fp = fopen(file,"a");
	int arr[50];
	
	length_server = sizeof(serv_addr);

		for(int i=0;i<numpackets;i++)
		{
			int T = TTL;
			memset(buffer,'\0',sizeof(buffer));
			createdatagram(buffer,i,T,Payload);
			while(T)
			{
				if(n = sendto(sockfd,buffer,Payload+7,0,(struct sockaddr*)&serv_addr,length_server ) == -1)
				{
					error("Sending Error!!!\n");
				}
				if(n = recvfrom(sockfd,buffer,Payload+7,0,(struct sockaddr*)&serv_addr,&length_server ) == -1)
				{
					error("Receive error!!!\n");
				}
				T = buffer[6] -1;
				buffer[6] -= 1;
			}
			rectime = timeStamp();
			printf("Cumulative RTT time for Payload %d for %d packet is %d\n",Payload,i,(rectime-sendtime));
			arr[i] = (rectime-sendtime);
		}
		for(int i=0;i<50;i++)
		{
			fprintf(fp,"%d,",arr[i]);
		}
		fprintf(fp,"\n");

	fclose(fp);

}
