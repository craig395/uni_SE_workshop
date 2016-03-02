#include "SetTableStatus.h"
#include <regex>


SetTableStatus::SetTableStatus(DatabaseHelper* dbHelper)
{
	pagePath = "/set-table-status";
	db = dbHelper;
	htmlCache = "<form action=\"\" method=\"post\"><fieldset><legend>Edit table status</legend>";
}


SetTableStatus::~SetTableStatus()
{
}

string SetTableStatus::runPage(PageRequest request)
{
	//Prepare regex match
	const regex numberCheck("^-?\\d+$");
	const regex statusCheck("(ready|inactive|in-use)");

	//check for form submission
	if (regex_match(request.getPostData("tableID"), numberCheck) && regex_match(request.getPostData("tableStatus"), statusCheck))
	{//Form submission
		//Prepare parameters
		vector<BindParam> params;
		params.push_back(BindParam(textType, request.getPostData("tableStatus")));
		params.push_back(BindParam(intType, request.getPostData("tableID")));

		//Run query
		db->runNoReturnQuery("UPDATE `Table` SET `Status` = ? WHERE `Table_Number` = ?", params);

		//return success 
		return "<center><h>Update Made</h></center>";
	}
	else
	{//No submission 
	 //Get list of open Tabs
		vector<vector<string>>* results = db->runQuery("SELECT `Table_Number` FROM `Table`");

		//Return form
		string formPart;

		//Table selection
		formPart += "<label>Table to update</label> <select name=\"tableID\">";//Start of selection
		for (auto i = results->begin(); i != results->end(); ++i)
		{
			formPart += "<option value = \"" + (*i)[0] + "\">Table Number: " + (*i)[0] + "</option>";
		}

		formPart += "</select><br>";//End of selection
		

		//Status selection
		formPart += "<label>Status</label> <select name=\"tableStatus\">";//Start of selection
		formPart += "<option value = \"ready\">Ready</option>";//Ready status
		formPart += "<option value = \"inactive\">Inactive</option>";//Inactive status
		formPart += "<option value = \"in-use\">In use</option>";//in-use Status
		formPart += "</select><br>";//End of selection


		//Submit button				
		formPart += "<input type=\"submit\" value=\"Update\">";
		formPart += "</fieldset></form>";


		delete results;
		return htmlCache + formPart;
	}
}
