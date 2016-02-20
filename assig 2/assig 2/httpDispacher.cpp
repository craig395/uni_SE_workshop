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
	stopAllThreads();
	deleteallRequests();
}

void httpDispacher::stopAllThreads()
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

void httpDispacher::deleteallRequests()
{
	//Delete all queued requests 
	while (queuedRequests.size() > 0)
	{
		threadInsruction* tmp = queuedRequests.front();
		queuedRequests.pop();
		SOCKET* tmpSocket = tmp->clientSocket;
		closesocket(*tmpSocket);
		delete tmpSocket;
		delete tmp;
	}
}



void httpDispacher::startThreads()
{
	//Populate array
	for (int i = 0; i < WORKER_THREAD_COUNT; i++) 
	{
		//Add the thread to the vecotr
		workerThreads.push_back(std::thread(&httpDispacher::workerThread, this));
	}
}

void httpDispacher::addThreadInstruction(SOCKET* clientSocket)
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
		tmp->inst = closeTheadWorker;
		tmp->clientSocket = nullptr;
		return tmp;
	}
	else
	{//Not closing down
		//Check if there are any queue requests
		if (queuedRequests.size() < 1)
		{//No queued requests
			dispacherMutex.unlock();
			threadInsruction* tmp = new threadInsruction();
			tmp->inst = noJob;
			tmp->clientSocket = nullptr;
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
	//TODO: head generator
	//TODO: webPage

	threadInsruction* tmpInstruction = new threadInsruction;
	tmpInstruction->clientSocket = nullptr;
	tmpInstruction->inst = noJob;

	//Loop until close is returned
	while (tmpInstruction->inst != closeTheadWorker)
	{
		//check type of instruction
		if (tmpInstruction->inst == reply)
		{//Instruction type is "reply to socket"
			SOCKET* tmpSocket = tmpInstruction->clientSocket;
			tmpInstruction->clientSocket = nullptr;
			//TODO: get response (HTTP request)


			//Clean up
			closesocket(*tmpSocket);
			delete tmpSocket;
			//tmpInstruction is deleted at the bottom of the loop
		}
		else if (tmpInstruction->inst == noJob)
		{//Instruction type is "no jobs available"
			Sleep(THREAD_SLEEP_TIME);
			//tmpInstruction is deleted at the bottom of the loop
		}

		delete tmpInstruction;
	}

	delete tmpInstruction;
	//get next instruction
	tmpInstruction = getThreadInstruction();
}

