#pragma once
#include <string>
#include <regex>
#include <vector>

using namespace std;

struct postItem {
	string name;
	string value;
};

struct passedHead {
	string url;
	string identifier;
	vector<postItem> postData;
};

class HtmlHeadProcessor
{
public:
	HtmlHeadProcessor();
	~HtmlHeadProcessor();
	passedHead pharseHead(string head);
private:
	regex urlRegex;
	regex postDataRegex;
};

