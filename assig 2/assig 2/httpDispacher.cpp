#include "httpDispacher.h"
#include <mutex>
#include <iostream>

#define WORKER_THREAD_COUNT 10
#define MAX_QUEUE_SIZE 20

httpDispacher::httpDispacher()
{
}


httpDispacher::~httpDispacher()
{
	//Close all worker threads
	closeThreads = true;
	for (auto i = workerThreads.begin(); i != workerThreads.end(); i++) 
	{
		auto& tmp = *i;
		tmp.join();
		//TODO: delete the thread
	}
}

void httpDispacher::startThreads()
{
	//Populate array
	for (int i = 0; i < WORKER_THREAD_COUNT; i++) 
	{
		//Add the thread to the vecotr
		workerThreads.push_back(std::thread(&httpDispacher::workerThread));
	}
}

void httpDispacher::addWaiterInstruction(SOCKET* clientSocket)
{
	dispacherMutex.lock();
	if (queuedRequests.size() > MAX_QUEUE_SIZE)
	{
		dispacherMutex.unlock();
		char* message = "Too many connections, try again later\n";//TODO: add http header
		send(*clientSocket, message, strlen(message), 0);

		//close connection
		closesocket(*clientSocket);
		
		delete clientSocket;
		return; 
	}
	else 
	{
		dispacherMutex.unlock();
		waiterInsruction* tmp = new waiterInsruction();
		tmp->clientSocket = clientSocket;

	}
}

void httpDispacher::workerThread()
{

}

