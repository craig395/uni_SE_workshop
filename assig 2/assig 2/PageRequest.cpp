#include "PageRequest.h"



PageRequest::PageRequest(map<string, string> post)
{
	postData = post;
}


PageRequest::~PageRequest()
{
}

pageId PageRequest::getRequestedPage()
{
	return requestedPage;
}

void PageRequest::setRequestedPage(pageId page)
{
	requestedPage = page;
}

string PageRequest::getPostData(string name)
{
	//Check if the post data exists
	if (postData.find(name) != postData.end())
	{//exists
		return postData[name];
	}
	else
	{//Does not exist
		return "";
	}
}
