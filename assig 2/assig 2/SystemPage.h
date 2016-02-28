#pragma once
#include <string>
#include "PageRequest.h"
#include "FileHelper.h"

using namespace std;

class SystemPage
{
public:
	SystemPage();
	~SystemPage();
	string getPagePath();
	virtual string runPage(PageRequest request);
protected:
	string pagePath;
	string htmlCache;
	FileHelper fileOperations;
};

