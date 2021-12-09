#pragma once

#include "Machine.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <boost/range/algorithm/count.hpp>
#include <boost/algorithm/string/find.hpp>

using namespace std;

class ConnectionHandler // TODO: static members
{
private:
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
private:
	// Query functions
	string GetString(int id, const char* name);
	int GetInt(int id, const char* name);
	// bool UpdateString(int id, const char* name, const char* s, const char* table = "data");
	// bool UpdateInt(int id, const char* name, int v, const char* table = "data");
	// bool UpdateDouble(int id, const char* name, double v, const char* table = "data");
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
public:

	// Constructors

	ConnectionHandler();
	ConnectionHandler(const char* server, const char* username, const char* password, const char* database);
	bool ConstructDataTable(bool checkPurchases = false); // Construct a datatable if one isn't presented

	// Connection functions

	bool ConnectToDatabase(const char* server, const char* username, const char* password, const char* database);
	bool Check(); // Checks the connection to the database and reconnects if the connection is down
	bool Reconnect();
	void Disconnect();

	// Processing functions

	// This entire section has to be moved to another class ConnectionHandler, which would be the child of Connection class
	string GetName(int id); // Get name of the machine
	vector<string> GetProducts(int id);
	map<string, int> GetAllIngredients(int id); // Get all of the ingrdients to check if they are in enough quantity
	map<string, int> GetNeededIngredients(int id, const char* product); // Get ingredients neede for product
	string GetMachineIngredients(int id); // Get ingredients used by machine
	string GetMachineIngredientsQuantities(int id); // Get ingredients quantities
	vector<string> GetIngredients(int id);
	vector<string> GetQuantities(int id);
	vector<map<string, int>> GetIngredientsVector(int id);
	vector<int> GetQuantity(int id);
	vector<double> GetPrices(int id);
	double GetPrice(int id, string name); // Get the price of the particular product

	bool CreateMachine(const char* name, const char* type, int& out_id); // Create a new machine, sets out_id to -1 if an error occured
	bool DeleteMachine(int id); // Delete machine
	bool SetName(int id, const char* name); // Set name of the machine
	bool AddItem(int id, string type, string name, string quantity, string price); // Add an non-ingredient item. It's uneffective and bad anyway if you're creating a new item with the same name.
	bool AddItem(int id, string type, string name, string ingredients, string ingredient_quantities, string quantity, string price); // Add an ingredient item. It's uneffective and bad anyway if you're creating a new item with the same name.
	bool AddIngredient(int id, string name, string quantity); // Add an ingredient to the available ingredients list
	bool TakeItem(int id, const char* name, int quantity); // Take an item from machine
	bool TakeIngredient(int id, const char* name, int quantity); // Take an ingredient from machine
	bool DeleteItem(int id, const char* name); // Delete an item
	bool DeleteIngredient(int id, const char* name); // Delete an ingredient
	bool LogPurchase(int id, const char* items, const char* prices, double total, const char* date, const char* time); // Log purchase

	// Checking functions

	bool IsColumnExisting(const char* column, const char* table); // Checks if a column exists
	bool IsDataTableExisting(const char* table, vector<const char*> columns); // Checks if a datatable exists and all the columns needed are presented there
	bool IsIdExisting(int id); // Checks if a machine with id exists
	bool CheckDBStructure(bool checkPurchases = false);
	bool CheckRowStructure(int id); // Checks if a row is correct in machine and ingredients tables, all strings are by data-quantity equal

	// Callbacks

	void OnConnectionError(); // Checks if MySQL connection is ok

	// Variable processing functions

	vector<string> split(string s, string delimiter);
	string back_split(vector<string> v, string delimiter);
	string back_split(vector<int> v, string delimiter);
	string back_split(vector<double> v, string delimiter);
};