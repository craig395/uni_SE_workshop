#include "PageController.h"



PageController::PageController()
{
}


PageController::~PageController()
{
}

string PageController::handleRequest(string pageName)
{
	//TODO: work out what user made the request

	//Work out what page is being requested
	pageId resolvedPage;
	string pageOutput;
	if (pageName == "\\")
	{
		resolvedPage = index;
	}
	else
	{
		//TODO: Search pages
		resolvedPage = unknown404;
	}

	//Return page or 404 page
	if (resolvedPage != unknown404)
	{//Other page
		PageRequest request;
		request.setRequestedPage(resolvedPage);

		pageOutput = runPage(request);
	}
	else
	{//404 page
		
	}

	//Merge with template if needed
	//TODO: above

	return pageOutput;
}

string PageController::runPage(PageRequest request)
{
	return "Hello there!";
}

bool PageController::loginUser(string username, string password)
{
	return false;
}

string PageController::mergeWithTemplate(string html)
{
	return string();
}
