#include "PageController.h"



PageController::PageController()
{
	//Cache style sheet
	styleSheetCache = fileOperatios.getTextfromFile("mystyle.css");

	//Cache template, split for easy insertion of other HTML
	string templateHTML = fileOperatios.getTextfromFile("mainTemplate.html");

	//Find location to split
	static string splitPoint = "<div class=\"content\">";
	int splitLocation = templateHTML.find(splitPoint) + splitPoint.size();

	//Split into the two string
	templateStartCache = templateHTML.substr(0, splitLocation);
	templateEndCache = templateHTML.substr(splitLocation + 1);

}


PageController::~PageController()
{
}

string PageController::handleRequest(passedHead request)
{
	//TODO: work out what user made the request

	//Work out what page is being requested
	pageId resolvedPage;
	string pageOutput;
	if (request.url == "/")
	{
		resolvedPage = index;
	}else if (request.url == "/mystyle.css")
	{
		resolvedPage = styleSheet;
	}
	else
	{
		//TODO: Search pages
		Orders orderPage;
		if (request.url == orderPage.getPagePath()) {
			resolvedPage = order;
		}
		else {
			resolvedPage = unknown404;
		}
	}


	//Check what type of page to return
	if (resolvedPage == unknown404)
	{//404 page

	}
	else if (resolvedPage == styleSheet)
	{//Style sheet
		return styleSheetCache;
	}
	else
	{//Other page
		PageRequest request;
		request.setRequestedPage(resolvedPage);

		pageOutput = runPage(request);
	}


	//Merge with template if needed
	//TODO: above
	if (request.postData.size() == 0)
	{//Encapsulate in template
		return templateStartCache + pageOutput + templateEndCache;
	}
	else
	{//Don't encapsulate in template
		return pageOutput;
	}

}

string PageController::runPage(PageRequest request)
{
	Orders orderPage;
	if (request.getRequestedPage() == order)
	{
		return orderPage.runPage(request);
	}
	else {
		return "Other Page";
	}
}

bool PageController::loginUser(string username, string password)
{
	return true;
}

string PageController::mergeWithTemplate(string html)
{
	return html;
}
