#include "httpServer.h"
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib") 

#define HTTP_PORT 80


httpServer::httpServer()
{
}


httpServer::~httpServer()
{
}

void httpServer::startListen()
{
	WSADATA wsa;
	SOCKET ServerSocket;
	SOCKET* clientSocket;
	struct sockaddr_in server, client;


	//Initializing winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{//Startup failed
		//failed with error: WSAGetLastError());
		//exit here
	}

	//Create a socket then check its valid
	if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{//Invalid socket
		//Socket creation failed message here: WSAGetLastError();
		//exit here
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(HTTP_PORT);

	//Bind socket
	if (bind(ServerSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{//Failed to bind socket
		//add bind failed error message here: WSAGetLastError();
	}

	//Create worker threads
	startThreads();
}




