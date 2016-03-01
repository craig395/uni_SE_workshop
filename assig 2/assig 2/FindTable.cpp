#include "FindTable.h"
#include <regex>


FindTable::FindTable(DatabaseHelper* dbHelper)
{
	pagePath = "/find-table";
	db = dbHelper;
	htmlCache = "<form action = \"\" method = \"post\"><fieldset><legend>Table Finder</legend><label>Number of Guests</label><input name = \"findTableGuests\"  type = \"number\" min = \"1\" value = \"\" required><input type = \"submit\" value = \"Find Table\"></fieldset></form>" ;

	//Cache HTML for list of tables

	//Cache preparation for List cache
	string htmlFile = fileOperations.getTextfromFile("findTable.html");

	//###Table Number###
	//Find location to split
	string splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Table Number</b></div>";
	int splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	listCache[0] = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Seats###
	//Find location to split
	splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Seats</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	listCache[1] = htmlFile.substr(0, splitLocation);
	htmlFile = htmlFile.substr(splitLocation + 1);

	//###Status###
	//Find location to split
	splitPoint = "<div class=\"orderItem orderItemTitle\"><b>Status</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	listCache[2] = htmlFile.substr(0, splitLocation);
	listCache[3] = htmlFile.substr(splitLocation + 1);
}


FindTable::~FindTable()
{
}

string FindTable::runPage(PageRequest request)
{
	//Prepare regex
	const regex numberCheck("^-?\\d+$");

	//Check if a number has been provided, and that it is a number
	if (regex_match(request.getPostData("findTableGuests"),numberCheck))
	{//Number is provided and is ok

		//Run query and output results
		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("findTableGuests")));
		vector<vector<string>>* results = db->runQuery("SELECT * FROM `Table` WHERE (`Seats` >= ?) AND (`Status` = 'ready') ORDER BY `Seats` ASC", params);


		//Used to store the HTML for each column
		string tableNumberColum;
		string seatsColum;
		string statusColum;

		//Div used to cover output
		const string listItemStart = "<div ";//TODO: on click load the next stage to setup ()onclick=\"post('/manage-table',{flag : 'form', tableID : ''}); \" 
		const string listItemStart2 = "class=\"orderItem manageItem\">";
		const string listItemEnd = "</div>";


		string output;
		//Check if there are any items
		if (results->size() > 0)
		{//Items in list
		 //Loop through items
			for (auto i = results->begin(); i != results->end(); ++i)
			{
				//Fill the columns that we can
				switch (i->size())
				{
				case 3:
					statusColum += listItemStart + listItemStart2 + i->at(2) + listItemEnd;
				case 2:
					seatsColum += listItemStart + listItemStart2 + i->at(1) + listItemEnd;
				case 1:
					tableNumberColum += listItemStart + listItemStart2 + i->at(0) + listItemEnd;
				}
			}

			output = listCache[0] + tableNumberColum + listCache[1] + seatsColum + listCache[2] + statusColum + listCache[3];
		}
		else
		{//No items in list
			output = "No items";//TODO: make it look nicer
		}


		delete results;
		return output;
		
	}
	else
	{//Nothing provided
		return htmlCache;
	}
	return string();
}
