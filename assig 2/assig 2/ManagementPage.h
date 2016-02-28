#pragma once
#include "SystemPage.h"
class ManagementPage :
	public SystemPage
{
public:
	ManagementPage(DatabaseHelper* dbHelper);
	~ManagementPage();
	string runPage(PageRequest request);
protected:
	virtual string generateList();
	virtual string prepareForm(int id);
private:
	string listCache[4];
};

