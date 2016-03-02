#pragma once
#include "SystemPage.h"
class SetTableStatus :
	public SystemPage
{
public:
	SetTableStatus(DatabaseHelper* dbHelper);
	~SetTableStatus();
	string runPage(PageRequest request);
};

