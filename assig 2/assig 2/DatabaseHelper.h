#pragma once
#include <mutex>
#include <vector>
#include "sqlite3.h" //SQLite was got from http://sqlite.org/

using namespace std;

enum DataType {intType, textType};

struct BindParam {
	DataType typeOfData;
	string value;
	BindParam(DataType type, string paramValue) {
		typeOfData = type;
		value = paramValue;
	}
};

class DatabaseHelper
{
public:
	DatabaseHelper(string databasePath);
	~DatabaseHelper();
	vector<vector<string>>* runQuery(string query);
	void runNoReturnQuery(string query, vector<BindParam> params);
private:
	mutex dbMutex;
	sqlite3* dbConnection = nullptr;
};

