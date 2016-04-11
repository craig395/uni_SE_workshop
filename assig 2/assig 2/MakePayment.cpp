#include "MakePayment.h"
#include <regex>


MakePayment::MakePayment(DatabaseHelper* dbHelper)
{
	pagePath = "/Make-Payment";
	db = dbHelper;
}


MakePayment::~MakePayment()
{
}

string MakePayment::runPage(PageRequest request)
{
	const regex numberCheck("^-?\\d+$");

	//Check submissions
	if (regex_match(request.getPostData("tabID"), numberCheck) && (request.getPostData("payment") == ""))
	{//Tab selected
		//Calculate price for all complete orders
		int total = calculateTotalCost(request.getPostData("tabID"));

		//Return total cost and payment option
		string output = "<center><h>Total cost: " + to_string(total) + "</h></center>";
		output += "<form action=\"\" method=\"post\"><fieldset><legend>Payment options</legend>";
		output += "<input type=\"hidden\" name=\"tabID\" value=\"" + request.getPostData("tabID") + "\">";
		output += "<input type=\"submit\" name=\"payment\" value=\"Cash\">";
		output += "<input type=\"submit\" name=\"payment\" value=\"Card\">";
		output += "</fieldset></form>";

		return output;
	}
	if (request.getPostData("payment") != "")
	{//Payment option received
		//Get needed details
		int total = calculateTotalCost(request.getPostData("tabID"));
		//get table_number
		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("tabID")));
		vector<vector<string>>* tabs = db->runQuery("SELECT `Table_Number` FROM `Tab` WHERE `ID` = ?", params);
		if (tabs->size() < 1)
		{//Nothing returned
			delete tabs;
			return "<center><h>Bad tabID</h></center>";
		}
		string table_number = (*tabs)[0][0];
		delete tabs;
		//Create timestamp
		time_t now = time(0);
		struct tm timeDate;
		localtime_s(&timeDate, &now);
		string timeStamp = to_string(timeDate.tm_mday) + "-" + to_string(1 + timeDate.tm_mon) + "-" + to_string(1900 + timeDate.tm_year) + "(" + to_string(1 + timeDate.tm_hour) + ":" + to_string(1 + timeDate.tm_min) + ":" + to_string(1 + timeDate.tm_sec) + ")";

		//Create payment record
		params.clear();
		params.push_back(BindParam(intType, request.getPostData("tabID")));
		params.push_back(BindParam(intType, to_string(total)));
		params.push_back(BindParam(intType, to_string(total)));
		params.push_back(BindParam(textType, request.getPostData("payment")));
		params.push_back(BindParam(textType, timeStamp));
		params.push_back(BindParam(intType, "1"));
		db->runNoReturnQuery("INSERT INTO `Payment` (`Tab_ID`, `Sub_Total`, `Total`, `Transaction_Type`, `Timestamp`, `Staff_ID`) VALUES (?,?,?,?,?,?)", params);

		//Close tab
		params.clear();
		params.push_back(BindParam(textType, timeStamp));
		params.push_back(BindParam(intType, request.getPostData("tabID")));
		db->runNoReturnQuery("UPDATE `Tab` SET `Closed_Timestamp`=? WHERE `ID`=?", params);

		//Update table status
		params.clear();
		params.push_back(BindParam(intType, table_number));
		db->runNoReturnQuery("UPDATE `Table` SET `Status`='ready' WHERE `Table_Number`=?", params);

		//Update order status to paid`Tab_ID` = ?
		params.clear();
		params.push_back(BindParam(intType, request.getPostData("tabID")));
		db->runNoReturnQuery("UPDATE `Order` SET `Status`='paid' WHERE `Tab_ID`=?", params);

		//Output success
		return "<center><h>Payment made successfully</h></center>";
	}
	else
	{//No submissions, first stage
		//Output tab selection form
		string output = generateTabSelectionForm();

		return output;
	}
}

string MakePayment::generateTabSelectionForm()
{
	vector<vector<string>>* tabs = db->runQuery("SELECT `ID`, `Name` FROM `Tab` WHERE `Closed_Timestamp`=''");

	//Check if there are results
	if (tabs->size() < 1)
	{//No tabs
		return "<center><h>No tabs</h></center>";
	}

	string output = "<form action=\"\" method=\"post\"><fieldset><legend>Tab selection</legend> <label>Tab to pay for</label> <select name=\"tabID\">";//Start of selection

	//Loop through all the tabs
	for (auto i = tabs->begin(); i != tabs->end(); ++i)
	{
		//Fill in the info Possible
		if (i->size() > 1)
		{//At least two parameters returned
			output += "<option value=\"" + (*i)[0] + "\">Name on Tab: " + (*i)[1] + "</option>";
		}
		else
		{//Only one parameter returned
			output += "<option value=\"" + (*i)[0] + "\">Tab ID: " + (*i)[0] + "</option>";
		}
	}
	output += "</select><br>";//End of selection

	output += "<input type=\"submit\" value=\"Submit\">";

	output += "</fieldset></form>";

	delete tabs;
	return output;
}

int MakePayment::calculateTotalCost(string tabID)
{
	vector<BindParam> params;
	params.push_back(BindParam(intType, tabID));
	vector<vector<string>>* orders = db->runQuery("SELECT `ID` FROM `Order` WHERE (`Status` = 'complete') AND (`Tab_ID` = ?)", params);

	int total = 0;
	for (auto i = orders->begin(); i != orders->end(); ++i)
	{//Loop through all the orders
		//Get all the items ordered in the order
		vector<BindParam> params2;
		params2.push_back(BindParam(intType, (*i)[0].c_str()));
		vector<vector<string>>* items = db->runQuery("SELECT `Sale_Item_ID`, `Quantity` FROM `Order_Item` WHERE (`Order_ID` = ?)", params2);
		for (auto a = items->begin(); a != items->end(); ++a)
		{
			//Get price of the sale_Item
			vector<BindParam> params3;
			params3.push_back(BindParam(intType, (*a)[0]));
			vector<vector<string>>* sale_item = db->runQuery("SELECT `Price` FROM `Sale_Item` WHERE (`ID` = ?)", params3);

			//Calculate cost( price * quantity)
			if (sale_item->size() > 0)
			{
				//Price * quantity
				total += atoi((*a)[1].c_str()) *  atoi((*sale_item)[0][0].c_str());
			}


			delete sale_item;
		}

		delete items;
	}

	delete orders;
	return total;
}
