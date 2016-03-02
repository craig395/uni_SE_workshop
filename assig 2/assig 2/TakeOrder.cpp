#include "TakeOrder.h"
#include <regex>


TakeOrder::TakeOrder(DatabaseHelper* dbHelper)
{
	pagePath = "/take-order";
	db = dbHelper;
	htmlCache = "<form action=\"\" method=\"post\"><fieldset><legend>Edit table status</legend>";
}


TakeOrder::~TakeOrder()
{
}

string TakeOrder::runPage(PageRequest request)
{
	const regex numberCheck("^-?\\d+$");

	//Check for form submissions to find stage of input
	if(regex_match(request.getPostData("tableID"), numberCheck))
	{//Post data, second Stage
		//Get list of tabs for the selected table
		vector<BindParam> params;
		params.push_back(BindParam(intType, request.getPostData("tableID")));
		vector<vector<string>>* results = db->runQuery("SELECT `ID`, `Name` FROM `Tab` WHERE (`Table_Number` = ?) AND `Closed_Timestamp` == ''", params);

		//Check for multiple open Tabs on the table number
		if (results->size() > 1)
		{//Multiple Tabs
			//output tab selection
			return generateTabSelectionForm(results);
		}
		else if (results->size() > 0)
		{//Single tab
			//Create order
			string newOrderID = createOrder((*results)[0][0]);
			delete results;
			if (newOrderID == "-1") return "<center><h>Bad table ID provided</h></center>";
			//Output order item form
			return generateOrderItemForm(newOrderID);
		}
		else
		{//No tabs returned for selected table
			delete results;
			return "<center><h>No tabs open for that table</h></center>";
		}

		
	}
	else if (regex_match(request.getPostData("tabID"), numberCheck) || (request.getPostData("button") != ""))
	{//order post data, third stage

		//Check for order item form submission 
		if (!regex_match(request.getPostData("tabID"), numberCheck))
		{//Order item form submission 
			//Create the submitted orderItem
			//Prepare parameters
			vector<BindParam> params;
			params.push_back(BindParam(intType, request.getPostData("saleItemID")));
			params.push_back(BindParam(intType, request.getPostData("orderID")));
			params.push_back(BindParam(intType, request.getPostData("numberOfItems")));

			db->runNoReturnQuery("INSERT INTO `Order_Item` (`Sale_Item_ID`, `Order_ID`, `Quantity`) VALUES (?,?,?)", params);

			//Check for final submission
			if (request.getPostData("button") != "SubmitAndEnd")
			{//Not final

			 //Output order item form
				return generateOrderItemForm(request.getPostData("saleItemID"));
			}
			else
			{//Final
				return "<center><h>Order made</h></center>";
			}

		}
		else
		{//Tab selection made
			//Create order
			string newOrderID = createOrder(request.getPostData("tabID"));
			//Output order item form
			return generateOrderItemForm(newOrderID);
		}
	}
	else
	{//No inputs, first stage
		//Get list of tables and output as a form
		return generateTableSelectionForm();
	}
}

string TakeOrder::generateTabSelectionForm(vector<vector<string>>* tabs)
{
	{
		//Check if there are results
		if (tabs->size() < 1)
		{//No tabs
			return "<center><h>No tabs</h></center>";
		}

		string output = "<form action = \"\" method=\"post\"><fieldset><legend>Tab selection</legend> <label>Tab to place order on</label> <select name=\"tabID\">";//Start of selection

		//Loop through all the tabs
		for (auto i = tabs->begin(); i != tabs->end(); ++i)
		{
			//Fill in the info Possible
			if (i->size() > 1)
			{//At least two parameters returned
				output += "<option value = \"" + (*i)[0] + "\">Name on Tab: " + (*i)[1] + "</option>";
			}
			else
			{//Only one parameter returned
				output += "<option value = \"" + (*i)[0] + "\">Tab ID: " + (*i)[0] + "</option>";
			}
		}
		output += "</select><br>";//End of selection
		
		output += "</fieldset></form>";

		delete tabs;
		return output;
	}
}

string TakeOrder::createOrder(string tabID)
{
	//Generate time-stamp
	time_t now = time(0);
	struct tm timeDate;
	localtime_s(&timeDate, &now);
	string timeStamp = to_string(timeDate.tm_mday) + "-" + to_string(1 + timeDate.tm_mon) + "-" + to_string(1900 + timeDate.tm_year) + "(" + to_string(1 + timeDate.tm_hour) + ":" + to_string(1 + timeDate.tm_min) + ":" + to_string(1 + timeDate.tm_sec) + ")";

	//Prepare parameters
	vector<BindParam> params;
	params.push_back(BindParam(intType, tabID));
	params.push_back(BindParam(intType, "1"));//TODO: get user id
	params.push_back(BindParam(textType, timeStamp));
	//Run query
	db->runNoReturnQuery("INSERT INTO `Order` (`Tab_ID`, `Staff_ID`, `Status`, `Timestamp`) VALUES (? , ? , 'in-prog', ?);", params);
	vector<vector<string>>* result = db->runQuery("SELECT `ID` FROM `Order` ORDER BY `ID` DESC LIMIT 1;");

	string output;
	if (result->size() > 0)
	{
		output = (*result)[0][0];
	}
	else
	{
		output = "-1";
	}
	
	delete result;
	return output;
}

string TakeOrder::generateOrderItemForm(string orderID)
{
	//Run order items query
	vector<vector<string>>* results = db->runQuery("SELECT `ID`, `Name`, `Price` FROM `Sale_Item`");

	string form = "<form action = \"\" method=\"post\"><fieldset><legendTake Order Form</legend>";

	//OrderID
	form += "<input name=\"orderID\" type=\"hidden\" value=\"" + orderID + "\" ><br>";
	//Number of items input
	form += "<label>Number of Items</label> <input name=\"numberOfItems\"  type=\"number\" min=\"1\" value=\"1\" required><br>";
	//Order Item selection
	form += "<label>Item selection</label> <select name=\"saleItemID\">";//Start of selection
	if (results->size() > 0)
	{//Not an empty vector
		for (auto i = results->begin(); i != results->end(); ++i)
		{
			form += "<option value = \"" + (*i)[0] + "\">" + "Name: " + (*i)[1] + " Price: " + (*i)[2] + "</option>";
		}
	}
	else
	{//Empty vector
		delete results;
		return "<center><h>No Items to order</h></center>";
	}

	delete results;
	form += "</select><br>";//End of selection

	//Submit buttons
	form += "<input type=\"submit\" name=\"button\" value=\"Submit and add new\">";
	form += "<input type=\"submit\" name=\"button\" value=\"SubmitAndEnd\">";


	form += "</fieldset></form>";
	return form;
}

string TakeOrder::generateTableSelectionForm()
{
	//Get list of tables
	vector<vector<string>>* results = db->runQuery("SELECT `Table_Number` FROM `Table`");

	string output = "<form action = \"\" method=\"post\"><fieldset><legend>Take Order Form</legend>";

	//Table selection
	output += "<label>Table </label> <select name=\"tableID\">";//Start of selection
	for (auto i = results->begin(); i != results->end(); ++i)
	{
		output += "<option value = \"" + (*i)[0] + "\">Table: " + (*i)[0] + "</option>";
	}
	delete results;
	output += "</select><br>";//End of selection
	//Submit button
	output += "<input type=\"submit\" value=\"Submit\">";

	output += "</fieldset></form>";
	return output;
}
