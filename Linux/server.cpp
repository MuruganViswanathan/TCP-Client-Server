#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORTNUM 12000
int main()
{

	//1. create socket
	//
	int server = socket(AF_INET, SOCK_STREAM, 0);
	if(server < 0)
	{
		printf("Error: creating socket!\n");
		return 1;
	}

	printf("Created socket...\n");

	//2. bind
	//
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(PORTNUM);

	if(bind(server, (struct sockaddr*)&my_addr, sizeof(my_addr)) != 0)
	{
		printf("Error: unable to bind!\n");
		close(server);
		return 1;
	}

	printf("bind successful...\n");

	//3. listen
	//
	if(listen(server, 3) != 0)
	{
		printf("Error: unable to listen!\n");
		close(server);
		return 1;
	}
	
	printf("listening on port %d ...\n", PORTNUM);

	//4. accept, send and receive data in while loop
	//
	struct sockaddr_in peer_addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	while(1)
	{
		static int i = 0;
		
		printf("waiting to accept connection from client... ");
		int connection = accept(server, (struct sockaddr*)&peer_addr, &addr_size);
		if(connection < 0)
		{
			printf("Error: accept failed !\n");
			close(server);
			return 1;
		}

		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
		printf("connection established with ip: %s and PORT: %d\n", ip, ntohs(peer_addr.sin_port));

		while(1)
		{
			i++;
			char buffer[256] = {};
			recv(connection, buffer, 256, 0);
			//read(connection, buffer, 256);
			printf("RECEIVED FROM CLIENT: %s\n", buffer);

			char buffer2[256] = {};
			sprintf(buffer2, "Hello back %d response for \"%s\"", i, buffer);
			send(connection, buffer2, 256, 0);
		//	sleep(1);
		}
	}

	return 0;
}
