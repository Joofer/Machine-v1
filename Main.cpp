// TODO
// 1. Add machine picking menu                                // Done
// 2. Add an ability to create a new machine in-runtime       // Done
// 3. Add purchases logging                                   // Done
// 4. Restructure ConnectionHandler class                     // Done
// 5. Add an ability to refill multiple items and ingredients // Done
// 6. Edit Buy function, so it takes error_code reference     // Done
// 7. Static runtime class                                    // Done
// 8. Display all machines                                    // Done
// 9. Wallets via hash files and database                     //
// 10. Add error logger                                       //
// 11. Add error logging switcher using define                //
// 12. Remove TODO comments from Main.cpp                     //

#include "libs/CoffeeMachine.h"
#include "libs/Wallet.h"
#include "libs/Runtime.h"
#include "libs/ConnectionHandler.h"

using namespace std;

bool UpdateData(int, ConnectionHandler*, Wallet*, Machine*);

int main()
{
	Machine* machine = new CoffeeMachine();
	Wallet* wallet = new Wallet();
	ConnectionHandler* connection = new ConnectionHandler();

	int machine_id = -1;

	string action = "";
	int error;

	bool isConnected = false;
	bool isPicked = false;

	// if (connection->ConnectToDatabase("tcp://joofer.mysql.database.azure.com:3306", "user", "YEAH!121_@YEAH!121_@", "database0")) // Connecting to database automatically
	// {
	// 	isConnected = true;
	// }

	while (!isConnected) // Connecting to database
	{
		Runtime::PrintConnect();

		cin >> action;

		switch (action[0])
		{
		case '1':
			if (Runtime::Connect(connection))
				isConnected = true;
			break;

		case 'x':
			exit(0);
			break;

		default:
			continue;
		}
	}

	// if (connection->CreateMachine("ChunkyCup", "coffee_machine", machine_id))
	// {
	// 	isPicked = true;
	// }

	while (!isPicked)
	{
		Runtime::PrintPick();

		cin >> action;

		switch (action[0])
		{
		case '1':
			if (Runtime::Pick(connection, machine_id))
				isPicked = true;
			break;

		case '2':
			if (!Runtime::Create(connection))
				Runtime::ThrowError(Error::ERROR_WHILE_MACHINE_CREATION); // Error while creating a new machine
			break;
		case '3':
			if (!Runtime::Delete(connection))
				Runtime::ThrowError(Error::NO_CONNECTION); // Error while deleting machine
			break;
		case '4':
			Runtime::DisplayAll(connection);
			break;
		case 'x':
			exit(0);
			break;

		default:
			continue;
		}
	}

	// cout << "Connected to database. Loading data." << endl;

	wallet->SetMoney(100); // Set up the wallet, it has to be done in the main function as it should be called only once

	while (action[0] != 'x')
	{
		if (!UpdateData(machine_id, connection, wallet, machine)) // Update machine
		{
			cout << "Error while updating the data." << endl;
			exit(10);
		}

		Runtime::PrintMain();
		cin >> action;

		switch (action[0])
		{
		case 'C':
			Runtime::PrintCoffee(machine);
			if (machine->GetItemsCount() > 0) // Check if we have any items before making the customer choose product
			{
				if (dp::read_string(action)) // Get product name
				{
					if (!Runtime::Buy(machine_id, wallet, machine, action, connection, error))
						Runtime::ThrowError(Error(error));
					else
						Runtime::PrintSubtraction(machine->FindItem(action)->GetPrice());

					Runtime::PrintMoney(wallet);
				}
				else
				{
					Runtime::ThrowError(Error::NO_CONNECTION);
				}
			}
			break;

		case 'H':
			break;

		case '1':
			Runtime::PrintManage(1);
			cin >> action;

			switch (action[0])
			{
			case 'N':
				if (!Runtime::AddProduct(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while adding a product
				break;

			case 'G':
				if (!Runtime::RefillItem(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while refilling an item
				break;

			case 'R':
				if (!Runtime::RemoveProduct(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while removing a product
				break;

			case 'D':
				Runtime::PrintProducts(machine);
				break;

			case 'M':
				Runtime::PrintName(machine);
				break;

			case 'C':
				if (!Runtime::SetName(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while removing a product
				break;

			default:
				break;
			}

			break;

		case '2':
			Runtime::PrintManage(2);
			cin >> action;

			switch (action[0])
			{
			case 'N':
				if (!Runtime::AddIngredient(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while adding an ingredient
				break;

			case 'I':
				if (!Runtime::RefillIngredient(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while refilling an ingredient
				break;

			case 'R':
				if (!Runtime::RemoveIngredient(machine_id, machine, connection))
					Runtime::ThrowError(Error::NO_CONNECTION); // Error while removing an ingredient
				break;

			case 'E':
				Runtime::PrintIngredients(machine);
				break;

			default:
				break;
			}

			break;

		default:
			break;
		}
	}

	return 0;
}

bool UpdateData(int machine_id, ConnectionHandler* connection, Wallet* wallet, Machine* machine)
{
	vector<string> products = connection->GetMachineItems(machine_id);
	vector<map<string, int>> ingredients = connection->GetMachineIngredientsVector(machine_id);
	vector<int> quantity = connection->GetItemQuantities(machine_id);
	vector<double> product_prices = connection->GetItemPrices(machine_id);

	MachineItem* temp_item;
	vector<MachineItem> items;

	if (!connection->Check()) // Checking connection
		return false;

	if (!connection->IsIdExisting(machine_id))
		return false;

	machine->ClearData(); // Clear local data

	// wallet->SetMoney(100); // Set up the wallet

	machine->SetIngredients(connection->GetAllIngredients(machine_id), true); // Set up ingredients
	machine->SetName((connection->GetMachineName(machine_id)).c_str()); // Set up machine's name

	if (products.size() != quantity.size() || quantity.size() != product_prices.size())
	{
		cout << "Error while getting data." << endl;
		return false;
	}

	for (int i = 0; i < products.size(); i++)
	{
		temp_item = new MachineItem(products[i], quantity[i], product_prices[i], ingredients[i].begin(), ingredients[i].end());
		items.push_back(*temp_item);
		temp_item = NULL;
	}

	machine->Add(items.begin(), items.end()); // Loading up the machine

	// cout << "Data loaded." << endl;

	return true;
}