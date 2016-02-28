#include "ManagementPage.h"
#include <vector>


ManagementPage::ManagementPage(DatabaseHelper* dbHelper)
{
	pagePath = "/manage-table";

	//Set HTML cache
	//TODO: this

	//set the db helper
	db = dbHelper;

	//Cache preparation for List cache
	string htmlFile = fileOperations.getTextfromFile("listTemplate.html");

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


ManagementPage::~ManagementPage()
{
}

string ManagementPage::runPage(PageRequest request)
{
	//Check if view item, delete item or view items
	if ((request.getPostData("flag") != "delete") && (request.getPostData("flag") != "form"))
	{//View items
		//Get list of items
		return generateList();

	}
	else if (request.getPostData("flag") == "delete")
	{//Remove item

	}
	else
	{//Edit/view/create item
		//TODO: check if posted data
		//TODO: check if there is an ID
		if (true)
		{//No ID
			return prepareForm(-1);
		}
		else
		{//ID provided
			//TODO: this
		}
	}
	return string();
}

string ManagementPage::generateList()
{
	//Get list of items
	vector<vector<string>>* results = db->runQuery("SELECT * FROM `Table`");
	//Used to store the HTML for each column
	string tableNumberColum;
	string seatsColum;
	string statusColum;

	//Div used to cover output
	const string listItemStart = "<div onclick=\"post('/manage-table',{flag : 'form'}); \" class=\"orderItem manageItem\">";
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
				statusColum += listItemStart + i->at(2) + listItemEnd;
			case 2:
				seatsColum += listItemStart + i->at(1) + listItemEnd;
			case 1:
				tableNumberColum += listItemStart + i->at(0) + listItemEnd;
			}
		}

		output = listCache[0] + tableNumberColum + listCache[1] + seatsColum + listCache[2] + statusColum + listCache[3];
	}
	else
	{//No items in list
		output = "No items";//TODO: make it look nicer
	}

	//Clean up
	delete results;

	//Return HTML
	return output;
}

string ManagementPage::prepareForm(int id)
{
	string seats = "1";
	string hiddenField;

	//Fill form or not
	if (id != -1)
	{//Fill form
		//Get data
		vector<vector<string>>* result = db->runQuery("SELECT `Seats` FROM `Table` WHERE `Table_Number` = " + id);//TODO: watch for injection

		//Fill strings
		if ((result->size() > 0) && ((*result)[0].size() > 0))
		{//There are results and a column
			seats = (*result)[0][0];
			hiddenField = "<input type=\"hidden\" name=\"id\" value=\"" + to_string(id) + "\">";
		}
		else
		{//ID not found
			hiddenField = "<input type=\"hidden\" name=\"id\" value=\"-1\">";
		}
	}
	else
	{
		hiddenField = "<input type=\"hidden\" name=\"id\" value=\"-1\">";
	}

	return "<form action = \"\" method = \"post\"><fieldset><legend>Table Details</legend><label>Number of seats</label><input name = \"seats\"  type = \"number\" min = \"1\" value = \"" + seats + "\" required>" + 
		"<input type=\"submit\" value=\"Save\"></fieldset></form>";
}
