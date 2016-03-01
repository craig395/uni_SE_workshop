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

vector<vector<string>>* DatabaseHelper::runQuery(string query, vector<BindParam> params)
{
	sqlite3_stmt* dbQuery = nullptr;

	//Prepare connection
	if(SQLITE_OK != sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &dbQuery, nullptr))
	{
		sqlite3_errmsg(dbConnection);//TODO: output this string to the error log
		return nullptr;
	}

	//Bind parameters
	int i = 0;
	for (auto itr = params.begin(); itr != params.end(); ++itr)
	{
		i++;
		if (itr->typeOfData == textType)
		{
			sqlite3_bind_text(dbQuery, i, itr->value.c_str(), -1, SQLITE_TRANSIENT);
		}
		else
		{
			//convert string to int
			int tmp = atoi(itr->value.c_str());//TODO: Check for errors
			sqlite3_bind_int(dbQuery, i, tmp);
		}
	}

	vector<vector<string>>* returnValue = new vector<vector<string>>();

	while(SQLITE_ROW == sqlite3_step(dbQuery))
	{
		vector<string> tmp;
		for (int i = 0; i < sqlite3_data_count(dbQuery); i++)
		{
			//Constant unsigned char* to string, then pushed into the vector
			tmp.push_back(reinterpret_cast<const char*>(sqlite3_column_text(dbQuery, i)));
		}

		returnValue->push_back(tmp);
	}


	//Cleanup
	sqlite3_finalize(dbQuery);//this deletes the pointer

	return returnValue;
}

void DatabaseHelper::runNoReturnQuery(string query, vector<BindParam> params)//TODO: change to return true or false based on success or failure
{
	sqlite3_stmt* dbQuery = nullptr;

	//Prepare connection
	if (SQLITE_OK != sqlite3_prepare_v2(dbConnection, query.c_str(), -1, &dbQuery, nullptr))
	{
		sqlite3_errmsg(dbConnection);//TODO: output this string to the error log
		return;
	}

	//Bind parameters
	int i = 0;
	for (auto itr = params.begin(); itr != params.end(); ++itr) 
	{
		i++;
		if (itr->typeOfData == textType)
		{
			sqlite3_bind_text(dbQuery, i, itr->value.c_str(), -1, SQLITE_TRANSIENT);
		}
		else
		{
			//convert string to int
			int tmp = atoi(itr->value.c_str());//TODO: Check for errors
			sqlite3_bind_int(dbQuery, i, tmp);
		}
	}

	//run query and clean up
	sqlite3_step(dbQuery);
	sqlite3_finalize(dbQuery);//this deletes the pointer
}
