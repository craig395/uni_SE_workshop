#pragma once
#include "PageEnum.h"

class PageRequest
{
public:
	PageRequest();
	~PageRequest();
	pageId getRequestedPage();
	void setRequestedPage(pageId page);

private:
	pageId requestedPage = index;
	//TODO: user type
};

