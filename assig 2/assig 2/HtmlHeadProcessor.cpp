#include "HtmlHeadProcessor.h"
using namespace std::regex_constants;

HtmlHeadProcessor::HtmlHeadProcessor() : urlRegex("^\\s*(?:GET\\s)(.+)(?:\\sHTTP)") , postDataRegex("(?:([A-z%0-9]+)\\s*=\\s*([A-z%0-9]+))") , requestTypeRegex("^\\s*POST\\s")
{
	
}


HtmlHeadProcessor::~HtmlHeadProcessor()
{
}

passedHead HtmlHeadProcessor::pharseHead(string head)
{
	passedHead processedHead;

	//Check URL is included and extract it
	if (regex_search(head, match, urlRegex)) 
	{//URL is included
		processedHead.url = match[1].str();
	}
	else
	{//URL is not included (Bad HTTP request)
		//presume index
		processedHead.url = "/";
	}

	//Check if the request is a GET or a POST
	if (regex_match(head,requestTypeRegex))
	{//Is a POST request
		//Get all the POST data
		while (std::regex_search(head, match, postDataRegex)) {
			if (match.size() == 2) {//TODO: tidy up
				postItem tmp;
				ssub_match sub_match = match[0];
				tmp.name = sub_match.str();
				sub_match = match[1];
				tmp.value = sub_match.str();
				processedHead.postData.push_back(tmp);
			}
		}
	}


	return processedHead;
}
