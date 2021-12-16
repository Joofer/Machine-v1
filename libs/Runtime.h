#pragma once

#include "Wallet.h"
#include "CoffeeMachine.h"
#include "BeverageMachine.h"
#include "WendingMachine.h"
#include "ConnectionHandler.h"
#include <chrono>
#include <ctime>
#include <sstream>

using namespace std;

enum class Error
{
	NOT_FOUND,
	NO_MONEY,
	NO_QUANTITY,
	NO_CONNECTION,
	ERROR_WHILE_MACHINE_CREATION,
	ERROR_WHILE_DELETING_MACHINE,
	ERROR_WHILE_LOGGING_PURCHASE
};

const int MAX_ITEM_QUANTITY = 100;
const int MAX_INGREDIENT_QUANTITY = 100;

class Runtime
{
private:
public:
	Runtime();

	// Machine picking and creation runtime

	static void PrintConnect();
	static void PrintPick();
	static bool Connect(ConnectionHandler* connection);
	static bool Pick(ConnectionHandler* connection, int& out_id); // Pick machine with id
	static bool Create(ConnectionHandler* connection); // Create a new machine
	static bool Delete(ConnectionHandler* connection); // Delete a machine
	static void DisplayAll(ConnectionHandler* connection); // Display all existing machines

	// In-machine runtime

	static bool Buy(int id, Wallet* wallet, Machine* machine, string item, ConnectionHandler* handler, int& error_code);
	static void ThrowError(Error error);
	static void PrintName(Machine* machine);
	static void PrintMoney(Wallet* wallet);
	static void PrintSubtraction(double value);
	static void PrintMain();
	static void PrintCoffee(Machine* machine);
	static void PrintManage(int panel);
	static void PrintProducts(Machine* machine);
	static void PrintIngredients(Machine* machine);
	static bool SetName(int id, Machine* machine, ConnectionHandler* handler);
	static bool AddProduct(int id, Machine* machine, ConnectionHandler* handler); // TODO: Add exceptions
	static bool AddIngredient(int id, Machine* machine, ConnectionHandler* handler);
	static bool RefillItem(int id, Machine* machine, ConnectionHandler* handler);
	static bool RefillIngredient(int id, Machine* machine, ConnectionHandler* handler);
	static bool RemoveProduct(int id, Machine* machine, ConnectionHandler* handler);
	static bool RemoveIngredient(int id, Machine* machine, ConnectionHandler* handler);
};