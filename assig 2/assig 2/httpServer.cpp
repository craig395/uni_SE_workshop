#include "httpServer.h"
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include "httpDispacher.h"

#pragma comment(lib,"ws2_32.lib") 

#define HTTP_PORT 80
#define SOCKET_BACKLOG 5

httpServer::httpServer()
{
}


httpServer::~httpServer()
{
	stopListening();
}

void httpServer::startListening()
{
	//Close the thread if its already running
	stopListening();
	//start the thread;
	listenerThread = new std::thread(&listener);
}

void httpServer::stopListening()
{
	//make sure there is a thread
	if (listenerThread != nullptr) 
	{
		//Let the thread know it needs to close
		stopThreadMutex.lock();
		stopThread = true;
		stopThreadMutex.unlock();

		//Wait for the thread to stop then delete it
		listenerThread->join();
		delete listenerThread;
		listenerThread = nullptr;

		//Reset the boolean now the threads stopped
		stopThreadMutex.lock();
		stopThread = false;
		stopThreadMutex.unlock();
	}
}

void httpServer::listener()
{
	WSADATA wsa;
	SOCKET ServerSocket;
	SOCKET* clientSocket;
	struct sockaddr_in server, client;
	httpDispacher dispacher;

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
	if (bind(ServerSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{//Failed to bind socket
	 //add bind failed error message here: WSAGetLastError();
	}

	//Create worker threads
	dispacher.startThreads();

	//Loop while running
	//Preceding mutex lock
	stopThreadMutex.lock();
	while (!stopThread)
	{
		stopThreadMutex.unlock();

		//Listen for any incoming connections
		listen(ServerSocket, SOCKET_BACKLOG);//TODO: Make non blocking



		stopThreadMutex.lock();
	}
	//Succeeding mutex unlock
	stopThreadMutex.unlock();

	//Clean up
}




