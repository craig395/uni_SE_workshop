#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <stdio.h>
#include <mutex>
#include <winsock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib") 

enum instruction{ reply, noJob, close };

struct threadInsruction
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
	void addThreadInstruction(SOCKET* clientSocket, std::string* clientRequest);
private:
	std::vector<std::thread> workerThreads;
	std::queue<threadInsruction*> queuedRequests;
	std::mutex dispacherMutex;
	threadInsruction* getThreadInstruction();
	void workerThread();
	bool closeThreads = false;
};

