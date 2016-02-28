#include "Orders.h"



Orders::Orders()
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
	splitPoint = "<div class=\"orderItem orderItemTitle orderItemRight\"><b>Time Stamp</b></div>";
	splitLocation = htmlFile.find(splitPoint) + splitPoint.size();
	//Split into the two string
	cacheParts[3] = htmlFile.substr(0, splitLocation);
	cacheParts[4] = htmlFile.substr(splitLocation + 1);

}


Orders::~Orders()
{
}


string Orders::runPage(PageRequest request)
{
	string a = "<div class=\"orderItem\">003003</div>";
	return htmlCache + cacheParts[0] + cacheParts[1] + cacheParts[2] + cacheParts[3] + cacheParts[4];
}
