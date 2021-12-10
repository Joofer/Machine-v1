#pragma once

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <boost/range/algorithm/count.hpp>
#include <boost/algorithm/string/find.hpp>
#include <vector>

using namespace std;

class Connection
{
protected:
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;

public:
	// Constructors

	Connection();
	Connection(const char* server, const char* username, const char* password, const char* database);
	bool ConstructDataTable(bool checkPurchases = false); // Construct a datatable if one isn't presented

	// Connection functions

	bool ConnectToDatabase(const char* server, const char* username, const char* password, const char* database);
	bool Check(); // Checks the connection to the database and reconnects if the connection is down
	bool Reconnect();
	void Disconnect();

	// Checking functions

	virtual bool IsColumnExisting(const char* column, const char* table) = 0; // Checks if a column exists
	virtual bool IsDataTableExisting(const char* table, vector<const char*> columns) = 0; // Checks if a datatable exists and all the columns needed are presented there
	virtual bool IsIdExisting(int id) = 0; // Checks if a machine with id exists
	virtual bool CheckDBStructure(bool checkPurchases = false) = 0;
	virtual bool CheckRowStructure(int id) = 0; // Checks if a row is correct in machine and ingredients tables, all strings are by data-quantity equal

	// Query functions

	string GetString(int id, const char* name, const char* table = "data");
	int GetInt(int id, const char* name, const char* table = "data");
	bool UpdateString(uint64_t id, const char* name, const char* s, const char* table = "data");
	bool UpdateInt(uint64_t id, const char* name, int v, const char* table = "data");
	bool UpdateDouble(uint64_t id, const char* name, double v, const char* table = "data");
	bool UpdateUInt64(uint64_t id, const char* name, uint64_t v, const char* table = "data");
	bool UpdateRow(int id, const char* products, const char* quantity, const char* product_prices); // Update row for non-ingredient machine
	bool UpdateRow(int id, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices); // Update row for ingredient machine
	bool Insert(const char* name, const char* type, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices, const char* ingredient_names, const char* ingredients_quantities, int& out_id); // Insert a row to the table, sets out_id to -1 if an error occured
	bool Insert(int machine_id, const char* items, const char* prices, double total, const char* date, const char* time); // Insert a row in purchase logging
	bool Delete(int id, const char* table = "data"); // Delete machine
	bool CheckQuery(int id); // Check if a query exists
	bool GetMaxId(int& out_id, const char* table = "data");
	bool GetMaxId(uint64_t& out_id, const char* table = "data");

	// Callbacks

	// void OnConnectionError(); // Checks if MySQL connection is ok
};