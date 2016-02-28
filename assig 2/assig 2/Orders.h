#pragma once
#include "SystemPage.h"

class Orders :
	public SystemPage
{
public:
	Orders(DatabaseHelper* dbHelper);
	~Orders();
	string runPage(PageRequest request);
private:
	string cacheParts[5];
};