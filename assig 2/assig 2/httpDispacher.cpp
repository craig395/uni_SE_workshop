#include "httpDispacher.h"
#include <iostream>

#define WORKER_THREAD_COUNT 10
#define MAX_QUEUE_SIZE 20
#define THREAD_SLEEP_TIME 10

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

	//Delete all queued requests 
	while (queuedRequests.size > 0)
	{
		threadInsruction* tmp = queuedRequests.front();
		queuedRequests.pop();
		delete tmp;
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

void httpDispacher::addThreadInstruction(SOCKET* clientSocket, std::string* clientRequest)
{
	dispacherMutex.lock();//Preventing access problems with multiple threads

	//Make sure that the limit on the queue size has not been met
	if (queuedRequests.size() > MAX_QUEUE_SIZE)
	{//Limit has been met
		dispacherMutex.unlock();
		char* message = "Too many connections, try again later\n";//TODO: add http header
		send(*clientSocket, message, strlen(message), 0);

		//close connection
		closesocket(*clientSocket);
		
		delete clientSocket;
		return; 
	}
	else 
	{//Limit has not been met
		dispacherMutex.unlock();
		threadInsruction* tmp = new threadInsruction();
		tmp->clientSocket = clientSocket;
		tmp->inst = reply;
		tmp->request = clientRequest;

		dispacherMutex.lock();
		queuedRequests.push(tmp);
		dispacherMutex.unlock();
	}
}

threadInsruction * httpDispacher::getThreadInstruction()
{
	dispacherMutex.lock();
	//check if shutting down
	if (closeThreads) 
	{//Closing down
		dispacherMutex.unlock();
		threadInsruction* tmp = new threadInsruction();
		tmp->inst = close;
		tmp->clientSocket = nullptr;
		tmp->request = nullptr;
		return tmp;
	}
	else
	{//Not closing down
	 //Check if there are any queue requests
		if (queuedRequests.size < 1)
		{//No queued requests
			dispacherMutex.unlock();
			threadInsruction* tmp = new threadInsruction();
			tmp->inst = noJob;
			tmp->clientSocket = nullptr;
			tmp->request = nullptr;
			return tmp;
		}
		else
		{//There are queued requests
			threadInsruction* tmp = queuedRequests.front();
			queuedRequests.pop();
			dispacherMutex.unlock();
			return tmp;
		}
	}
}



void httpDispacher::workerThread()
{
	//Preload objects
	//TODO:(create outside but static??)

	//TODO: head parser
	//TODO: webPage

	threadInsruction* tmpInstruction = new threadInsruction;
	tmpInstruction->clientSocket = nullptr;
	tmpInstruction->request = nullptr;
	tmpInstruction->inst = noJob;

	//Loop until close is returned
	while (tmpInstruction->inst != noJob)
	{
		//check type of instruction
		if (tmpInstruction->inst == reply)
		{//Instruction type is "reply to socket"

		}
		else if (tmpInstruction->inst == noJob)
		{//Instruction type is "no jobs available"
			Sleep(THREAD_SLEEP_TIME);
		}
	}
}

