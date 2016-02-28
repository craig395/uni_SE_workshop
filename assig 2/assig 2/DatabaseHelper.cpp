#include "DatabaseHelper.h"


DatabaseHelper::DatabaseHelper(string databasePath)
{
	//Make connection to the DB file
	if(SQLITE_OK != sqlite3_open(databasePath.c_str(), &dbConnection)) 
	{
		sqlite3_errmsg(dbConnection);//TODO: output this to the error log
		sqlite3_close(dbConnection);
		throw;
	}
}


DatabaseHelper::~DatabaseHelper()
{
	sqlite3_close(dbConnection);
}

vector<vector<string>>* DatabaseHelper::runQuery(string query)
{
	sqlite3_stmt* dbQuery = nullptr;

	if(SQLITE_OK != sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &dbQuery, nullptr))
	{
		sqlite3_errmsg(dbConnection);//TODO: output this string to the error log
		return nullptr;
	}

	vector<vector<string>>* returnValue = new vector<vector<string>>();

	while(SQLITE_ROW == sqlite3_step(dbQuery))
	{
		vector<string> tmp;
		for (int i = 0; i < sqlite3_data_count(dbQuery); i++)
		{
			//Const unsighned char* to string, then pushed into the vector
			tmp.push_back(reinterpret_cast<const char*>(sqlite3_column_text(dbQuery, i)));
		}

		returnValue->push_back(tmp);
	}

	sqlite3_finalize(dbQuery);

	return returnValue;
}
