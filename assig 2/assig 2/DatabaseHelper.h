#pragma once
#include <mutex>
#include <vector>
#include "sqlite3.h" //SQLite was got from http://sqlite.org/

using namespace std;

class DatabaseHelper
{
public:
	DatabaseHelper(string databasePath);
	~DatabaseHelper();
	vector<vector<string>>* runQuery(string query);
private:
	mutex dbMutex;
	sqlite3* dbConnection = nullptr;
};

