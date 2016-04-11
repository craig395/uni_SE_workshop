#include "FileHelper.h"
#include <iostream>
#include <fstream>


FileHelper::FileHelper()
{
}


FileHelper::~FileHelper()
{
}

string FileHelper::getTextfromFile(string file)
{
	ifstream htmlFile;
	string output;

	htmlFile.open(file);

	if (htmlFile.is_open()) {
		getline(htmlFile, output);
		while (htmlFile)
		{
			string tmp;
			output += "\n";
			getline(htmlFile, tmp);
			output += tmp;
		}
	}
	else
	{
		cout << "File access error!!";
	}

	return output;

}
