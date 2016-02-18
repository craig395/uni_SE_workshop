#pragma once
#include <thread>
#include <mutex>

#pragma comment(lib,"ws2_32.lib") 


class httpServer
{
public:
	httpServer();
	~httpServer();
	void startListening();
	void stopListening();
private:
	void listener();
	std::thread* listenerThread = nullptr;
	bool stopThread = false;
	std::mutex stopThreadMutex;
};

