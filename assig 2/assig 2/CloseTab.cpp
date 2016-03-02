#include "CloseTab.h"
#include <regex>


CloseTab::CloseTab(DatabaseHelper* dbHelper)
{
	pagePath = "/close-tab";
	db = dbHelper;
	htmlCache = "<form action=\"\" method=\"post\"><fieldset><legend>Close Tab Form</legend>";
}


CloseTab::~CloseTab()
{
}

string CloseTab::runPage(PageRequest request)
{
	//Prepare regex match
	const regex numberCheck("^-?\\d+$");

	//check for form submission
	if (regex_match(request.getPostData("tabID"), numberCheck))
	{//Form submission
		//Create time-stamp
		time_t now = time(0);
		struct tm timeDate;
		localtime_s(&timeDate, &now);
		string timeStamp = to_string(timeDate.tm_mday) + "-" + to_string(1 + timeDate.tm_mon) + "-" + to_string(1900 + timeDate.tm_year) + "(" + to_string(1 + timeDate.tm_hour) + ":" + to_string(1 + timeDate.tm_min) + ":" + to_string(1 + timeDate.tm_sec) + ")";


		//Prepare parameters
		vector<BindParam> params;
		params.push_back(BindParam(textType, timeStamp));
		params.push_back(BindParam(intType, request.getPostData("tabID")));

		//Run query
		db->runNoReturnQuery("UPDATE `Tab` SET `Closed_Timestamp` = ? WHERE `ID` = ?", params);

		//return success 
		return "<center><h>Tab closed</h></center>";
	}
	else
	{//No submission 
		//Get list of open Tabs
		vector<vector<string>>* results = db->runQuery("SELECT `ID`, `Name` FROM `Tab` WHERE `Closed_Timestamp` = ''");

		//Return form
		string formPart;

		//Tab selection
		formPart += "<label>Name on tab</label> <select name=\"tabID\">";//Start of selection
		for (auto i = results->begin(); i != results->end(); ++i)
		{
			formPart += "<option value = \"" + (*i)[0] + "\">Name: " + (*i)[1] + "</option>";
		}

		formPart += "</select><br>";//End of selection
									//Submit button
		formPart += "<input type=\"submit\" value=\"Close\">";


		formPart += "</fieldset></form>";


		delete results;
		return htmlCache + formPart;
	}
	return string();
}
