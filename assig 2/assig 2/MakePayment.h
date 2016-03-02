#pragma once
#include "SystemPage.h"
class MakePayment :
	public SystemPage
{
public:
	MakePayment(DatabaseHelper* dbHelper);
	~MakePayment();
	string runPage(PageRequest request);
private:
	string generateTabSelectionForm();
	int calculateTotalCost(string tabID);
};

