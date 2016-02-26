#pragma once
#include "PageRequest.h"
#include "HtmlHeadProcessor.h"
#include <string>
#include "FileHelper.h"
#include "Orders.h"

using namespace std;

class PageController
{
public:
	PageController();
	~PageController();
	string handleRequest(passedHead pageName);
private:
	string runPage(PageRequest request);
	bool loginUser(string username, string password);
	string mergeWithTemplate(string html);
	FileHelper fileOperatios;
	string styleSheetCache;
	string templateStartCache;
	string templateEndCache;
};

