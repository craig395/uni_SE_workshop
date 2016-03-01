#pragma once
#include "SystemPage.h"
class OpenTab :
	public SystemPage
{
public:
	OpenTab(DatabaseHelper* dbHelper);
	~OpenTab();
	string runPage(PageRequest request);
};

