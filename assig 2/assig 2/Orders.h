#pragma once
#include "SystemPage.h"
class Orders :
	public SystemPage
{
public:
	Orders();
	~Orders();
	string runPage(PageRequest request);
	string cacheParts[5];

};