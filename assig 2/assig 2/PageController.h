#pragma once
#include "PageRequest.h"
#include <string>

using namespace std;

class PageController
{
public:
	PageController();
	~PageController();
	string handleRequest(string pageName);
private:
	string runPage(PageRequest request);
	bool loginUser(string username, string password);
	string mergeWithTemplate(string html);
};

