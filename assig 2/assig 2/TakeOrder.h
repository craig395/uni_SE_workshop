#pragma once
#include "SystemPage.h"
class TakeOrder :
	public SystemPage
{
public:
	TakeOrder(DatabaseHelper* dbHelper);
	~TakeOrder();
	string runPage(PageRequest request);
private:
	string generateTabSelectionForm(vector<vector<string>>* tabs);
	string createOrder(string tabID);
	string generateOrderItemForm(string orderID);
	string generateTableSelectionForm();
};

