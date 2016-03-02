#pragma once
#include "PageEnum.h"
#include <map>

using namespace std;

class PageRequest
{
public:
	PageRequest(map<string, string> post);
	~PageRequest();
	pageId getRequestedPage();
	void setRequestedPage(pageId page);
	string getPostData(string name);
	

private:
	pageId requestedPage = indexID;
	map<string, string> postData;
	//TODO: user type
};

