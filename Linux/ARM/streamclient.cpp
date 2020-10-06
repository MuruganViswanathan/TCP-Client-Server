#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include<sys/un.h> 
//#include<netdb.h> 

int main(int argc, char* argv[])
{
	//validate 
	if(argc != 3)
	{
		printf("Usage: %s <server IP addr> <server PORT number>\n", argv[0]);
		return 1;
	}

	char ip[INET_ADDRSTRLEN];
	strncpy(ip, argv[1], INET_ADDRSTRLEN);
	unsigned short port = atoi(argv[2]);
	printf("Connecting to server: %s, port %d\n", ip, port);
	
    //0. Init
    //   create data buffer to receive data
    char* databuffer = 0;
    const unsigned int DATASIZE = sizeof(char)*2*1024*1024;
    databuffer = (char*) malloc (DATASIZE);
    if(databuffer == NULL)
    {
        printf("Error: could not allocate memory of size: %u !\n", DATASIZE);
        return 1;
    }

	//1. create socket
	//
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("Error: socket creating failed!\n");
		return 1;
	}
	printf("socket created successfully\n");


	//2. bind
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(12010);
	if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) != 0)
	{
		printf("Error: bind error!\n");
		close(sockfd);
		return 1;
	}
	printf("bind successful...\n");

	//3. connect to server
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);
	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("Error: could not connect to server\n");
		close(sockfd);
		return 1;
	}
	printf("server connected...\n");

        //4. send and receive
        //
        static int i = 0;
        while(1)
        {
                i++;

           //     char buffer[256] = {};
           //     sprintf(buffer, "Hello %d from client", i);
           //     send(sockfd, buffer, 256, 0);
           //     //write(sockfd, buffer, 256);
           //     //printf("sent: %s\n", buffer);

           //     char buffer2[256] = {};
           //     recv(sockfd, buffer2, 256, 0);
           //     printf("RECEIVED FROM SERVER: %s\n", buffer2);

                  int nsize = recv(sockfd, databuffer, DATASIZE, 0);
                  printf("RECEIVED FROM SERVER: %d - %d bytes\n", i, nsize);
       }


	return 0;
}

