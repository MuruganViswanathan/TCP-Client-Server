/*
 * Description: Start a TCP Client to connect to a TCP Server at <ipaddress>.
 * Receive packets from Server and do network performance profiling
 *
 * $DateTime: 2020/10/01 12:34:56 $
 * $Revision: #1 $
 *   $Author: murugan $
 *
 */

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
#include <sys/time.h>
//#include <time.h>

#define PORTNUM 12000
int main(int argc, char* argv[])
{
	//validate 
	if(argc != 2)
	{
		printf("Usage: %s <server IP addr> \n", argv[0]);
		return 1;
	}


	char ip[INET_ADDRSTRLEN];
	strncpy(ip, argv[1], INET_ADDRSTRLEN);
	unsigned short port = PORTNUM;//atoi(argv[2]);
	printf("Connecting to server: %s, port %d\n", ip, port);
	
    //0. Init
    //   create data buffer to receive data
    char* databuffer = 0;
    const unsigned int DATASIZE = sizeof(char)*4*1024;
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


    //4. receive N packets and calculate transfer rate
    //
	while(1)
    {
        static int i;
        i = 0;
        int N = 1000; //number of packets
        unsigned long int bytes_received = 0;
        struct timeval start, end;

        gettimeofday(&start, NULL);
        while(i < N)
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

                  bytes_received += recv(sockfd, databuffer, DATASIZE, 0);
                  //printf("RECEIVED FROM SERVER: %d - %ld bytes\n", i, bytes_received);
        }
        gettimeofday(&end, NULL);

        //if 0 bytes read continiously, terminate client
        if(bytes_received == 0)
        {
            static int bcount = 0;
            bcount++;
            if(bcount == 5)
            {
                printf("Zero bytes received. check if server is running... exiting client.\n");
                break;
            }
        }

        long int totalusec = (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);
        double total_sec = ((double)totalusec)/1000000;

        double rate = bytes_received/total_sec;
        double rateMbps = (rate*8)/1000000;

        printf("%ld bytes received in 0.3%f seconds. \t Transfer rate = 0.3%f bytes per sec ( %f Mbps )\n",
               bytes_received, total_sec, rate, rateMbps);
    }

	//99. cleanup
	//    free malloc
	free(databuffer);
	return 0;
}

