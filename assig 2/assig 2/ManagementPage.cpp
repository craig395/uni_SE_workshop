#include "ManagementPage.h"
#include <vector>
#include <regex>

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
	//Prepare regex match
	const regex numberCheck("^-?\\d+$");

	//Check if view item, delete item or view items
	if ((request.getPostData("flag") != "delete") && (request.getPostData("flag") != "form"))
	{//View items
		//Get list of items
		return generateList();

	}
	else if (request.getPostData("flag") == "delete")
	{//Remove item

		//Make sure the table ID is provide and that its a number
		if (regex_match(request.getPostData("tableID"), numberCheck))
		{//ID is provided and ok
			removeRecord(request.getPostData("tableID"));
		}

		return generateList();
	}
	else
	{//Edit/view/create item

		if (!regex_match(request.getPostData("tableID"), numberCheck))
		{//No ID
			return prepareForm("-1");
		}
		else
		{//ID provided
			//Check for form submission
			if (request.getPostData("tableSeats") != "")
			{//Form submission
				if (updateRecord(request))
				{//Update/create OK
					return generateList();
				}
				else
				{//Update/create Failed
					return "<b>Faild to update/create object</b>" + prepareForm(request.getPostData("tableID"));
				}
			}
			else
			{//No form submission
				return prepareForm(request.getPostData("tableID"));
			}
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
	const string listItemStart = "<div onclick=\"post('/manage-table',{flag : 'form', tableID : '";
	const string listItemStart2 = "'}); \" class=\"orderItem manageItem\">";
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
				statusColum += listItemStart + i->at(0) + listItemStart2 + i->at(2) + listItemEnd;
			case 2:
				seatsColum += listItemStart + i->at(0) + listItemStart2 + i->at(1) + listItemEnd;
			case 1:
				tableNumberColum += listItemStart + i->at(0) + listItemStart2 + i->at(0) + listItemEnd;
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

string ManagementPage::prepareForm(string id)
{
	string seats = "1";
	string hiddenField;
	string hiddenField2;

	//Fill form or not
	if (id != "-1")
	{//Fill form
		//Get data
		vector<vector<string>>* result = db->runQuery("SELECT `Seats` FROM `Table` WHERE `Table_Number` = " + id);//TODO: watch for injection

		//Fill strings
		if ((result->size() > 0) && ((*result)[0].size() > 0))
		{//There are results and a column
			seats = (*result)[0][0];
			hiddenField = "<input type=\"hidden\" name=\"tableID\" value=\"" + id + "\">";
		}
		else
		{//ID not found
			hiddenField = "<input type=\"hidden\" name=\"tableID\" value=\"-1\">";
			id = "-1";
		}

		delete result;
	}
	else
	{
		hiddenField = "<input type=\"hidden\" name=\"tableID\" value=\"-1\">";
	}

	hiddenField2 = "<input type=\"hidden\" name=\"flag\" value=\"form\">";

	string output = "<form action = \"\" method = \"post\"><fieldset><legend>Table Details</legend>" + hiddenField + hiddenField2 + "<label>Number of seats</label><input name = \"tableSeats\"  type = \"number\" min = \"1\" value = \"" + seats + "\" required>" +
		"<input type=\"submit\" value=\"Save\"></fieldset></form>";

	if (id != "-1") 
	{//Not a create new form
		hiddenField2 = "<input type=\"hidden\" name=\"flag\" value=\"delete\">";

		//Add Delete button to the output
		output += "<form action = \"\" method = \"post\"><fieldset><legend>Delete Record</legend>" + hiddenField + hiddenField2 +
			"<input type=\"submit\" value=\"Delete\"></fieldset></form>";
	} 
		
		return output;
}

bool ManagementPage::updateRecord(PageRequest request)
{
	//make sure that all required data is included
	if (request.getPostData("tableSeats") == "")
	{
		return false;
	}

	//Check if update or insert(ID is provided or not)
	if (request.getPostData("tableID") != "-1")
	{//Update record, ID provided
		//Prepare parameters
		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("tableSeats")));
		params.push_back(BindParam(intType, request.getPostData("tableID")));

		//Run query
		db->runNoReturnQuery("UPDATE `Table` SET `Seats` = ? WHERE `Table_Number` = ?", params);

		return true;
	}
	else
	{//Create new record, No ID provided
	 //Prepare parameters
		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("tableSeats")));
		params.push_back(BindParam(textType, "ready"));

		//Run query
		db->runNoReturnQuery("INSERT INTO `Table` (`Seats`, `Status`) VALUES (? , ?)", params);

		return true;
	}
}

void ManagementPage::removeRecord(string id)
{
	//Prepare parameters
	vector<BindParam> params;
	params.push_back(BindParam(intType, id));

	//Run query
	db->runNoReturnQuery("DELETE FROM `Table` WHERE `Table_Number` = ?;", params);
}
