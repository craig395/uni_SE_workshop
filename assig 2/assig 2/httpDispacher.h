#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <stdio.h>
#include <mutex>
#include <winsock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib") 

enum instruction{ reply, noJob, closeTheadWorker };

struct threadInsruction
{
	SOCKET* clientSocket;
	instruction inst;
};


class httpDispacher
{
public:
	httpDispacher();
	~httpDispacher();
	void stopAllThreads();
	void deleteallRequests();
	void startThreads();
	void addThreadInstruction(SOCKET* clientSocket);
private:
	std::vector<std::thread> workerThreads;
	std::queue<threadInsruction*> queuedRequests;
	std::mutex dispacherMutex;
	threadInsruction* getThreadInstruction();
	void workerThread();
	bool closeThreads = false;
};

