#pragma once
#include <thread>
#include <vector>
#include <stdio.h>
#include <winsock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib") 

enum instruction{ reply, noJob, close };

struct waiterInsruction
{
	std::string* request;
	SOCKET* clientSocket;
	instruction inst;
};


class httpDispacher
{
public:
	httpDispacher();
	~httpDispacher();
	void startThreads();
private:
	std::vector<std::thread> workerThreads;
	std::vector<waiterInsruction> queuedRequests;
	std::mutex dispacherMutex;
	void addWaiterInstruction(SOCKET* clientSocket);
	void workerThread();
	bool closeThreads = false;
};

