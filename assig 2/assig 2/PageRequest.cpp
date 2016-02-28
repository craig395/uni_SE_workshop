#include "PageRequest.h"



PageRequest::PageRequest()
{
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
