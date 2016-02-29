#pragma once
#include "SystemPage.h"
class ManagementPage :
	public SystemPage
{
public:
	ManagementPage(DatabaseHelper* dbHelper);
	~ManagementPage();
	string runPage(PageRequest request);
private:
	string generateList();
	string prepareForm(string id);
	bool updateRecord(PageRequest request);
	void removeRecord(string id);
	string listCache[4];
};

