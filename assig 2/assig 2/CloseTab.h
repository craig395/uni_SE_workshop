#pragma once
#include "SystemPage.h"
class CloseTab :
	public SystemPage
{
public:
	CloseTab(DatabaseHelper* dbHelper);
	~CloseTab();
	string runPage(PageRequest request);
};

