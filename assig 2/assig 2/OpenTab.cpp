#include "OpenTab.h"
#include <regex>

OpenTab::OpenTab(DatabaseHelper* dbHelper)
{
	pagePath = "/open-tab";
	db = dbHelper;
	htmlCache = "<form action=\"\" method=\"post\"><fieldset><legend>Open Tab Form</legend>";
}


OpenTab::~OpenTab()
{
}

string OpenTab::runPage(PageRequest request)
{
	//Prepare regex match
	const regex numberCheck("^-?\\d+$");

	//check for form submission
	if ((request.getPostData("createTabName") != "") && (regex_match(request.getPostData("createTabGuests"), numberCheck)) && (regex_match(request.getPostData("createTabTable"), numberCheck)))
	{//Form submission
		//Add tab to database
		//Create time stamp
		time_t now = time(0);
		struct tm timeDate;
		localtime_s(&timeDate, &now);
		string timeStamp = to_string(timeDate.tm_mday) + "-" + to_string(1 + timeDate.tm_mon) + "-" + to_string(1900 + timeDate.tm_year) + "(" + to_string(1 + timeDate.tm_hour) + ":" + to_string(1 + timeDate.tm_min) + ":" + to_string(1 + timeDate.tm_sec) + ")";

		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("createTabTable")));//Table_Number
		params.push_back(BindParam(textType, timeStamp));//Opened_Timestamp
		params.push_back(BindParam(intType, request.getPostData("createTabGuests")));//Number_Of_Guests
		params.push_back(BindParam(textType, request.getPostData("createTabName")));//Name

		params.push_back(BindParam(intType, "1"));

		db->runNoReturnQuery("INSERT INTO `Tab` (`Table_Number`, `Opened_Timestamp`, `Number_Of_Guests`, `Name`, `Staff_ID`, `Closed_Timestamp`) VALUES (? , ? , ? , ?, ?, '')", params);

		return "<center><h>Form Submitted</h></center>";
	}
	else
	{//No form submission
		//Get list of tables
		vector<vector<string>>* results = db->runQuery("SELECT `Table_Number` FROM `Table`");
		//Check for some provided values
		string table = request.getPostData("tableNumber");

		//Return form
		string formPart;

		//Number of guests input
		formPart = "<label>Number of Guests</label> <input name=\"createTabGuests\"  type=\"number\" min=\"1\" value=\"1\" required><br>";
		//Name input
		formPart += "<label>Name of Guest</label> <input name=\"createTabName\"  type=\"text\" required><br>";
		//Table selection
		formPart += "<label>Table </label> <select name=\"createTabTable\">";//Start of selection
		for (auto i = results->begin(); i != results->end(); ++i)
		{
			formPart += "<option ";
			if ((*i)[0] == table)
			{
				formPart += "selected ";
			}

			formPart += "value = \"" + (*i)[0] + "\">Table: " + (*i)[0] + "</option>";
		}

		formPart += "</select><br>";//End of selection
		//Submit button
		formPart += "<input type=\"submit\" value=\"Create\">";


		formPart += "</fieldset></form>";


		delete results;
		return htmlCache + formPart;
	}
}
