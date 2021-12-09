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

class RuntimeListener
{
private:
public:
	RuntimeListener();

	// Machine picking and creation runtime

	void PrintConnect();
	void PrintPick();
	bool Connect(ConnectionHandler* connection);
	bool Pick(ConnectionHandler* connection, int& out_id);
	bool Create(ConnectionHandler* connection); // Create a new machine
	bool Delete(ConnectionHandler* connection); // Delete a machine

	// In-machine runtime

	static int Buy(int id, Wallet* wallet, Machine* machine, string item, ConnectionHandler* handler);
	void ThrowError(Error error);
	void PrintName(Machine* machine);
	void PrintMoney(Wallet* wallet);
	void PrintSubtraction(double value);
	void PrintMain();
	void PrintCoffee(Machine* machine);
	void PrintManage(int panel);
	void PrintProducts(Machine* machine);
	void PrintIngredients(Machine* machine);
	bool SetName(int id, Machine* machine, ConnectionHandler* handler);
	bool AddProduct(int id, Machine* machine, ConnectionHandler* handler); // TODO: Add exceptions
	bool AddIngredient(int id, Machine* machine, ConnectionHandler* handler);
	bool RefillItem(int id, Machine* machine, ConnectionHandler* handler);
	bool RefillIngredient(int id, Machine* machine, ConnectionHandler* handler);
	bool RemoveProduct(int id, Machine* machine, ConnectionHandler* handler);
	bool RemoveIngredient(int id, Machine* machine, ConnectionHandler* handler);

	//

	bool read_int(int& out);
	bool read_double(double& out);
	bool read_string(string& out);
};