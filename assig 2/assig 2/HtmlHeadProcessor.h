#pragma once
#include <string>
#include <regex>
#include <map>

using namespace std;


struct passedHead {
	string url;
	string identifier;
	map<string, string>postData;
};

class HtmlHeadProcessor
{
public:
	HtmlHeadProcessor();
	~HtmlHeadProcessor();
	passedHead pharseHead(string head);
private:
	regex urlRegex;
	regex requestTypeRegex;
	regex postDataRegex;
	smatch match;
};

