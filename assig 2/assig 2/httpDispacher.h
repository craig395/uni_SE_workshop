#pragma once
#include <thread>
#include <vector>

class httpDispacher
{
public:
	httpDispacher();
	~httpDispacher();
	void startThreads();
private:
	std::vector<std::thread> workerThreads;
	void workerThread();
	bool closeThreads = false;
};

