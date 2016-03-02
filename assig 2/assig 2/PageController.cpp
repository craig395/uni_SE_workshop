#include "PageController.h"
#include "Orders.h"
#include "ManagementPage.h"
#include "FindTable.h"
#include "OpenTab.h"
#include "CloseTab.h"

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

	//Create the DB helper, one instance to prevent multiple reads at once, this object is thread safe
	dbHelper = new DatabaseHelper("database.db");

	//Fill list of pages
	pages[orderID] = new Orders(dbHelper);
	pages[tableID] = new ManagementPage(dbHelper);
	pages[findTableID] = new FindTable(dbHelper);
	pages[openTabID] = new OpenTab(dbHelper);
	pages[closeTabID] = new CloseTab(dbHelper);
}


PageController::~PageController()
{
	//Delete all pages
	//TODO: why?????
	for (auto i = pages.begin(); i != pages.end(); ++i)
	{
		delete (*i).second;
	}

	//delete pages[order];
	//delete pages[table];

	//Delete the db helper
	delete dbHelper;
}

string PageController::handleRequest(passedHead request)
{
	//TODO: work out what user made the request

	//Work out what page is being requested
	pageId resolvedPage;
	string pageOutput;
	if (request.url == "/")
	{
		resolvedPage = indexID;
	}else if (request.url == "/mystyle.css")
	{
		resolvedPage = styleSheetID;
	}
	else
	{
		resolvedPage = unknown404;

		//Loop through all pages
		for (auto i = pages.begin(); i != pages.end(); ++i)
		{
			//Check if this page is the one being requested
			if (request.url == i->second->getPagePath())
			{//It is
				resolvedPage = i->first;
				//No need to continue the loop so exit
				break;//TODO: as if considered bad in this case
			}
		}
	}


	//Check what type of page to return
	if (resolvedPage == unknown404)
	{//404 page

	}
	else if (resolvedPage == styleSheetID)
	{//Style sheet
		return styleSheetCache;
	}
	else
	{//Other page
		PageRequest request(request.postData);
		request.setRequestedPage(resolvedPage);

		pageOutput = runPage(request);
	}


	//Merge with template if needed
	//TODO: above
	if (request.postData.find("ajaxFlag") == request.postData.end())
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
	if (request.getRequestedPage() != indexID) {
		return pages[request.getRequestedPage()]->runPage(request);
	}
	else
	{
		return "Index page";
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
