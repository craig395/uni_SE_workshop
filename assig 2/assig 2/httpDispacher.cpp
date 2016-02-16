#include "httpDispacher.h"
#include <mutex>
#include <iostream>

#define WORKER_THREAD_COUNT 10


httpDispacher::httpDispacher()
{
}


httpDispacher::~httpDispacher()
{
	//Close all worker threads
	closeThreads = true;
	for (auto i = workerThreads.begin(); i != workerThreads.end(); i++) {
		auto& tmp = *i;

	}
}

void httpDispacher::startThreads()
{
	//Populate array
	for (int i = 0; i < WORKER_THREAD_COUNT; i++) {
		//Add the thread
		workerThreads.push_back(std::thread(&httpDispacher::workerThread));
	}
}

void httpDispacher::workerThread()
{

}

