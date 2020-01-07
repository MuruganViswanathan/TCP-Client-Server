// TCPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(int argc, char *argv[])
{

	//validate args
	if (argc != 3)
	{
		printf("Usage: %s <server IP addr> <server PORT number>\n", argv[0]);
		return 1;
	}

	char ip[20];
	strncpy_s(ip, argv[1], 20);
	unsigned short port = atoi(argv[2]);
	printf("Connecting to server: %s port %d\n", ip, port);

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	printf("Windows sock DLL status is %s\n", wsadata.szSystemStatus);


	//1. create socket
	//
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == INVALID_SOCKET)
	{
		printf("Error: socket creation failed!\n");
		WSACleanup();
		return 1;
	}
	printf("socket created successfully\n");


	//2. bind
	// //not needed - choose auto


	//3. connect to server
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = inet_addr(ip);
	inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("Error: could not connect to server\n");
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}

	getsockname(sockfd, (SOCKADDR*)&serv_addr, (int*) sizeof(serv_addr));
	char sip[20];
	inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, sip, 20);
	printf("server %s port %d connected...\n", sip, htons(serv_addr.sin_port));


	//4. send and receive
	//
	while (1)
	{
		static int i = 0;
		i++;

		char buffer[256] = {};
		sprintf_s(buffer, "Hello %d from client", i);
		if (send(sockfd, buffer, strlen(buffer), 0) == SOCKET_ERROR)
		{
			printf("Error: send() error %d\n", WSAGetLastError());
		}

		char buffer2[256] = {};
		recv(sockfd, buffer2, 256, 0);
		printf("RECEIVED FROM SERVER: %s\n", buffer2);

	}

	shutdown(sockfd, SD_SEND);
	closesocket(sockfd);
	WSACleanup();

	return 0;
}
