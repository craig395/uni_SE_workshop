#pragma once
#include "SystemPage.h"
class FindTable :
	public SystemPage
{
public:
	FindTable(DatabaseHelper* dbHelper);
	~FindTable();
	string runPage(PageRequest request);
private:
	string listCache[4];
};

