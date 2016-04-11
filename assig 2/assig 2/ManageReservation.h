#pragma once
#include "SystemPage.h"
class ManageReservation :
	public SystemPage
{
public:
	ManageReservation(DatabaseHelper* dbHelper);
	~ManageReservation();
	string runPage(PageRequest request);
private:
	string generateList();
	string prepareForm(string id);
	bool updateRecord(PageRequest request);
	void removeRecord(string id);
	string listCache[4];

};

