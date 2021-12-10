#pragma once

#include "Machine.h"
#include "Connection.h"
#include "DataProcessing.h"

using namespace std;

class ConnectionHandler : public Connection
{
public:
	// Checking functions

	bool IsColumnExisting(const char* column, const char* table); // Checks if a column exists
	bool IsDataTableExisting(const char* table, vector<const char*> columns); // Checks if a datatable exists and all the columns needed are presented there
	bool IsIdExisting(int id); // Checks if a machine with id exists
	bool CheckDBStructure(bool checkPurchases = false);
	bool CheckRowStructure(int id); // Checks if a row is correct in machine and ingredients tables, all strings are by data-quantity equal

	// Processing functions

	string GetMachineName(int id); // Get name of the machine
	vector<string> GetMachineItems(int id); // Get all items from machine
	map<string, int> GetAllIngredients(int id); // Get all of the ingredients to check if they are in enough quantity
	map<string, int> GetNeededIngredients(int id, const char* product); // Get ingredients neede for product
	string GetIngredients(int id); // Get ingredients used by machine
	string GetIngredientQuantities(int id); // Get ingredients quantities
	vector<string> GetMachineIngredients(int id); // Get all ingredients used by machine
	vector<string> GetMachineIngredientQuantities(int id); // Get all quantities used by machine
	vector<map<string, int>> GetMachineIngredientsVector(int id); // Get all ingredients by machine as vector of ingredient and quantity
	vector<int> GetItemQuantities(int id); // Get item quantities
	vector<double> GetItemPrices(int id); // Get item prices
	double GetItemPrice(int id, string name); // Get the price of the particular item

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
};