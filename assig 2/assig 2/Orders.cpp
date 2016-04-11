#include "Orders.h"
#include <vector>


Orders::Orders(DatabaseHelper* dbHelper)
{
	pagePath = "/orders";

	//Cache preparation
	string htmlFile = fileOperations.getTextfromFile("order.html");

	//###OrderID###
	//Find location to split
	string splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Order ID</b></div>";
	int splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	htmlCache = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Tab ID###
	//Find location to split
	splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Tab ID</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	cacheParts[0] = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Staff Member###
	//Find location to split
	splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Staff Member</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	cacheParts[1] = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Status###
	//Find location to split
	splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Status</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	cacheParts[2] = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Time stamp###
	//Find location to split
	splitPoint = "<b>Timestamp</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	cacheParts[3] = htmlFile.substr(0, splitLocation);
	cacheParts[4] = htmlFile.substr(splitLocation + 1);


	//Set database
	db = dbHelper;

	string a;
}


Orders::~Orders()
{
}


string Orders::runPage(PageRequest request)
{
	//Start and end divs for ech item 
	string listItemStart = "<div class=\"orderItem\">";
	string listItemEnd = "</div>";

	//Strings to hold columns of info
	vector<vector<string>>* results = db->runQuery("SELECT * FROM `Order`;");
	string orderIdList;
	string tabIdList;
	string staffMemberList;
	string statusList;
	string timestampList;

	for (auto i = results->begin(); i != results->end(); ++i)
	{
		//Fill the columns that we can
		switch (i->size())
		{
		case 5:
			timestampList += listItemStart + i->at(4) + listItemEnd;
		case 4:
			statusList += listItemStart + i->at(3) + listItemEnd;
		case 3:
			staffMemberList += listItemStart + i->at(2) + listItemEnd;
		case 2:
			tabIdList += listItemStart + i->at(1) + listItemEnd;
		case 1:
			orderIdList += listItemStart + i->at(0) + listItemEnd;
		}
	}

	delete results;

	return htmlCache + orderIdList + cacheParts[0] + tabIdList + cacheParts[1] + staffMemberList + cacheParts[2] + statusList + cacheParts[3] + timestampList + cacheParts[4];
}
