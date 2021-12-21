#include "libs\Runtime.h"

Runtime::Runtime()
{

}

//

void Runtime::PrintConnect()
{
	cout << "=====================================" << endl
		<< "\t\tCONNECT TO DATABASE" << endl
		<< "" << endl
		<< "1\tConnect to database" << endl
		<< "" << endl
		<< "x\tExit" << endl
		<< "=====================================" << endl;
}

void Runtime::PrintPick()
{
	cout << "=====================================" << endl
		<< "\t\tENTRY" << endl
		<< "" << endl
		<< "1\tConnect to an existing machine" << endl
		<< "2\tCreate a new machine" << endl
		<< "3\tDelete an existing machine" << endl
		<< "4\tDisplay all existing machines" << endl
		<< "" << endl
		<< "x\tExit" << endl
		<< "=====================================" << endl;
}

bool Runtime::Connect(ConnectionHandler* connection)
{
	string server;
	string user;
	string password;
	string database;

	Debug::Log("Connecting to machine...", true);

	cout << "Server: ";
	if (!dp::read_string(server))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	cout << "User: ";
	if (!dp::read_string(user))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	cout << "Password: ";
	if (!dp::read_string(password))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	cout << "Database: ";
	if (!dp::read_string(database))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (connection->ConnectToDatabase(server.c_str(), user.c_str(), password.c_str(), database.c_str())) // Connecting to database
	{
		Debug::Log("Successfully connected.");
		return true;
	}
	else
	{
		Debug::Log("Error while connecting to machine.", true);
		return false;
	}
}

bool Runtime::Pick(ConnectionHandler* connection, int& out_id)
{
	int id;

	Debug::Log("Picking machine...");

	cout << "Machine id: ";
	if (!dp::read_int(id))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (connection->IsIdExisting(id))
	{
		out_id = id;
		Debug::Log("Machine #" + to_string(id) + " was successfully picked.");
		return true;
	}
	else
	{
		Debug::Log("Machine with ID " + to_string(id) + " doesn't exist.");
		return false;
	}
}

bool Runtime::Create(ConnectionHandler* connection)
{
	int id;
	int temp_type;
	string name;
	string type;

	Debug::Log("Creating new machine...");

	cout << "=====================================" << endl
		<< "\t\tCREATE NEW MACHINE" << endl
		<< "=====================================" << endl;

	cout << "Machine name: ";
	if (!dp::read_string(name))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	cout << "Machine type (Beverage Machine - 1, Coffee Machine - 2, Wending Machine - 3): ";
	if (!dp::read_int(temp_type))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (temp_type < 1 || temp_type > 3)
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	switch (temp_type)
	{
	case 1:
		type = "beverage_machine";
		break;
	case 2:
		type = "coffee_machine";
		break;
	case 3:
		type = "wending_machine";
		break;
	default:
		return false;
	}

	if (connection->CreateMachine(name.c_str(), type.c_str(), id))
	{
		if (id != -1)
		{
			Debug::Log("Machine was successfully created. Machine's id: " + to_string(id) + ".", true);
			return true;
		}
		else
		{
			Debug::Log("Processing error, action aborted.", true);
			return false;
		}
	}
	else
	{
		Debug::Log("Processing error, action aborted.", true);
		return false;
	}
}

bool Runtime::Delete(ConnectionHandler* connection)
{
	int id;

	Debug::Log("Deleting machine...");

	cout << "Machine id: ";
	if (!dp::read_int(id))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}
	
	if (connection->IsIdExisting(id))
	{
		if (connection->DeleteMachine(id))
		{
			Debug::Log("Machine #" + to_string(id) + " was successfully deleted.", true);
			return true;
		}
		else
		{
			Debug::Log("Error while deleting machine # " + to_string(id) + ".", true);
			return false;
		}
	}
	else
	{
		Debug::Log("Machine with ID " + to_string(id) + " doesn't exist.");
		return false;
	}

}

void Runtime::DisplayAll(ConnectionHandler* connection)
{
	vector<string> names = dp::split(connection->GetMachineNames(), ";");

	cout << "=====================================" << endl
		<< "\t\tMACHINES" << endl
		<< "ID\t\tNAME" << endl;

	for (int i = 0; i < names.size(); i++)
		cout << i + 1 << "\t" << names[i] << endl;

	cout << "=====================================" << endl;
}

//

bool Runtime::Buy(int id, Wallet* wallet, Machine* machine, string item, ConnectionHandler* handler, int& error_code)
{
	map<string, int> ingredients;
	string prices;
	char price[255];

	// Date and time for logging

	char date_str[255];
	char time_str[255];
	
	sprintf_s(date_str, "%s-%s-%s", Datetime::GetYear().c_str(), Datetime::GetMonth().c_str(), Datetime::GetDay().c_str());
	sprintf_s(time_str, "%s:%s:%s", Datetime::GetHour().c_str(), Datetime::GetMin().c_str(), Datetime::GetSec().c_str());

	//

	// Getting price
	
	// for (int i = 0; i < 1; i++) // For multiple products
	// {
	// 	sprintf_s(price, "%g", machine->FindItem(item)->GetPrice());
	// 
	// 	prices += price;
	// 	if (i != 1)
	// 		prices += ";";
	// }
	sprintf_s(price, "%g", machine->FindItem(item)->GetPrice());
	prices = price;

	//

	Debug::Log("Trying to buy from Machine #" + to_string(id) + " item " + item + ", prices: " + prices + ", total_price: " + to_string(machine->FindItem(item)->GetPrice()) + ".");

	if (machine->FindId(item) != -1)
	{
		if (wallet->GetMoney() < machine->FindItem(item)->GetPrice())
		{
			error_code = 1; // Not enough money
			return false;
		}
		else
		{
			if (handler->TakeItem(id, item.c_str(), 1)) // We first check database and try to take an item from there
			{
				if (machine->Take(item))
				{
					ingredients = handler->GetNeededIngredients(id, item.c_str());

					for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
						if (!handler->TakeIngredient(id, (i->first).c_str(), i->second))
							Debug::Log("Error while updating database with values: " + to_string(id) + "\t" + i->first + "\t" + to_string(i->second));

					// Logging

					// Logging to database
					if (!handler->LogPurchase(id, item.c_str(), prices.c_str(), machine->FindItem(item)->GetPrice(), date_str, time_str))
						Debug::Log("Error while logging purchase.");

					// Logging locally
					Debug::Log("Machine #" + to_string(id) + ". Bought " + item + ", prices: " + prices + ", total_price: " + to_string(machine->FindItem(item)->GetPrice()) + ".");

					//

					wallet->TakeMoney(machine->FindItem(item)->GetPrice());
					error_code = -1;
					return true;
				}
				else
				{
					handler->TakeItem(id, item.c_str(), -1);

					error_code = 2; // Couldn't take an item from machine
					return false;
				}
			}
			else
			{
				error_code = 3; // Couldn't take an item from database
				return false;
			}
		}
	}
	else
	{
		error_code = 0; // Item not found
		return false;
	}
}

void Runtime::ThrowError(Error error)
{
	string error_str;

	switch (error)
	{
	case Error::NOT_FOUND:
		error_str = "Item not found.";
		break;
	case Error::NO_MONEY:
		error_str = "Not enough money.";
		break;
	case Error::NO_QUANTITY:
		error_str = "Not enough quantity.";
		break;
	case Error::NO_CONNECTION:
		error_str = "No connection with database or error while processing.";
		break;
	case Error::ERROR_WHILE_MACHINE_CREATION:
		error_str = "Error while creating a new machine.";
		break;
	case Error::ERROR_WHILE_DELETING_MACHINE:
		error_str = "Error while deleting a machine.";
		break;
	case Error::ERROR_WHILE_LOGGING_PURCHASE:
		error_str = "Error while logging purchase.";
		break;
	default:
		error_str = "Error while processing.";
		break;
	}

	error_str = "Error: " + error_str;

	Debug::Log(error_str.c_str(), true);
}

void Runtime::PrintName(Machine* machine)
{
	cout << "=====================================" << endl
		<< "CURRENT NAME: " << machine->GetName() << endl
		<< "=====================================" << endl;
}

void Runtime::PrintMoney(Wallet* wallet)
{
	cout << "You have " << wallet->GetMoney() << endl;
}

void Runtime::PrintSubtraction(double value)
{
	cout << " - " << value << endl;
}

void Runtime::PrintMain()
{
	cout << "=====================================" << endl
		<< "C\tBuy a cup of hot drink" << endl
		// << "H\tBuy a cup of hot chocolate" << endl // Another machine for hot chocolate
		<< "" << endl
		<< "1\tManage machine" << endl
		<< "2\tManage ingredients" << endl
		<< "" << endl
		<< "x\tExit machine" << endl
		<< "=====================================" << endl;
}

void Runtime::PrintCoffee(Machine* machine)
{
	MachineItem* current_item = new MachineItem();
	string machine_name = machine->GetName();
	int positions_max = 36;

	if (machine_name.size() < positions_max) // Just some machine name's fitting maths
	{
		int positions_left = (positions_max - machine_name.size()) / 2;

		for (int i = 0; i < positions_left; i++)
			cout << "=";

		cout << " " << machine->GetName() << " ";

		for (int i = 0; i < positions_left; i++)
			cout << "=";

		cout << endl;
	}
	else
	{
		cout << machine->GetName() << endl;
	}

	if (machine->GetItemsCount() < 1)
	{
		cout << "No items found. Create new ones" << endl
			<< "via the managing menus." << endl;
		cout << "=====================================" << endl;
	}
	else
	{
		for (int i = 0; i < machine->GetItemsCount(); i++)
		{
			current_item = machine->GetItemById(i);
			cout << current_item->Get() << "\tBuy a cup of " << current_item->Get() << "\t" << current_item->GetPrice() << endl;
		}
		cout << "=====================================" << endl;
		cout << "Product: ";
	}
}

void Runtime::PrintManage(int panel)
{
	switch (panel)
	{
	case 1:
		cout << "=====================================" << endl
			<< "\tMANAGE MENU:" << endl
			<< "N\tAdd new product" << endl
			<< "G\tRefill item" << endl
			<< "R\tRemove product" << endl
			<< "D\tDisplay all products" << endl
			<< "" << endl
			<< "M\tDisplay machine name" << endl
			<< "C\tRename machine" << endl
			<< "=====================================" << endl;
		break;

	case 2:
		cout << "=====================================" << endl
			<< "\tMANAGE MENU:" << endl
			<< "N\tAdd new ingredient" << endl
			<< "I\tRefill ingredient" << endl
			<< "R\tRemove ingredient" << endl
			<< "E\tDisplay all ingredients" << endl
			<< "=====================================" << endl;
		break;

	default:
		cout << "Unknown panel" << endl;
		break;
	}
}

void Runtime::PrintProducts(Machine* machine)
{
	cout << "=====================================" << endl
		<< "\tAVAILABLE ITEMS" << endl;

	if (machine->GetItemsCount() < 1)
	{
		cout << "No items found. Create new ones" << endl
			<< "via the managing menus." << endl;
	}
	else
	{
		cout << "#\tName\t\tPrice\t\tQuantity" << (typeid(*machine) == typeid(CoffeeMachine) ? "\t\tIngredients" : "") << endl;
		for (int i = 0; i < machine->GetItemsCount(); i++)
		{
			cout << i + 1 << ".\t" << machine->GetItemById(i)->Get() << "\t\t" << machine->GetItemById(i)->GetPrice() << "\t\t" << machine->GetItemById(i)->GetQuantity() << "\t\t" << (typeid(*machine) == typeid(CoffeeMachine) ? machine->GetItemById(i)->GetIngredients() : "") << endl;
		}
	}
	cout << "=====================================" << endl;
}

void Runtime::PrintIngredients(Machine* machine)
{
	map<string, int> ingredients;
	int k = 0;

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);
		return;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	cout << "=====================================" << endl
		<< "\tAVAILABLE INGREDIENTS:" << endl;

	if (ingredients.size() < 1)
	{
		cout << "No ingredients found. Create new ones" << endl
			<< "via the managing menus." << endl;
	}
	else
	{
		for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
		{
			cout << k + 1 << ". " << i->first << "\t" << i->second << endl;
			k++;
		}
	}
	cout << "=====================================" << endl;
}

bool Runtime::SetName(int id, Machine* machine, ConnectionHandler* handler)
{
	string name;

	Debug::Log("Setting up new name for machine #" + to_string(id) + "...");

	PrintName(machine);

	cout << "New name: ";
	if (!dp::read_string(name))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (handler->SetName(id, name.c_str()))
	{
		Debug::Log("Successfully set up new name \"" + name + "\" for machine #" + to_string(id) + ".");
		return true;
	}
	else
	{
		Debug::Log("Error while setting up new name for machine #" + to_string(id) + ".");
		return false;
	}
}

bool Runtime::AddProduct(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	map<string, int>::iterator iter;
	int ingredient_index = -1;
	int product_type_number;
	string product_type;
	string product_name;
	int product_ingredients_count;
	string product_ingredients = "";
	string product_ingredient_temp;
	string product_ingredient_quantities = "";
	int product_ingredient_quantity_temp;
	int product_quantity;
	double product_price;

	Debug::Log("Adding new product for machine #" + to_string(id) + "...");

	cout << "=====================================" << endl
		<< "\tADD PRODUCT:" << endl;

	cout << "Product type (Beverage Machine - 1, Coffee Machine - 2, Wending Machine - 3): ";
	if (!dp::read_int(product_type_number))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	switch (product_type_number)
	{
	case 1:
		product_type = "beverage_machine";
		break;
	case 2:
		product_type = "coffee_machine";
		break;
	case 3:
		product_type = "wending_machine";
		break;
	default:
		Debug::Log("Input error, action aborted.", true);
		return false;
	}
	
	cout << "Product name: ";
	if (!dp::read_string(product_name))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	product_name = dp::remove_from(product_name, ','); // Deleting delimiter characters from product_name
	product_name = dp::remove_from(product_name, ';'); // if they were typed

	if (product_type_number == 2)
	{
		if (typeid(*machine) != typeid(CoffeeMachine))
		{
			ThrowError(Error::NO_CONNECTION);
			return false;
		}

		ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

		cout << "Ingredients quantity: ";
		if (!dp::read_int(product_ingredients_count))
		{
			Debug::Log("Input error, action aborted.", true);
			return false;
		}

		PrintIngredients(machine);

		for (int i = 0; i < product_ingredients_count; i++)
		{
			cout << "Ingredient index " << i + 1 << ": ";
			if (!dp::read_int(ingredient_index))
			{
				Debug::Log("Input error, action aborted.", true);
				return false;
			}

			if (ingredient_index <= ingredients.size() && ingredient_index > 0)
			{
				iter = ingredients.begin(); // Setting up iterator
				advance(iter, ingredient_index - 1); // Increasing iterator position by ingredient_index picked - 1, as earlier it was increased
				product_ingredient_temp = iter->first;
			}
			else
			{
				Debug::Log("Error while picking ingredient.", true);
				return false; // ingredient_index is a too big or a too small value
			}

			if (i != product_ingredients_count - 1)
				product_ingredients = product_ingredients + product_ingredient_temp + ",";
			else
				product_ingredients += product_ingredient_temp;

			cout << "Quantity: ";
			if (!dp::read_int(product_ingredient_quantity_temp))
			{
				Debug::Log("Input error, action aborted.", true);
				return false;
			}

			if (product_ingredient_quantity_temp <= 0 || product_ingredient_quantity_temp >= MAX_INGREDIENT_QUANTITY) // Check if ingredient quantity input is correct
			{
				Debug::Log("Ingredient quantity is incorrect.", true);
				return false;
			}

			if (i != product_ingredients_count - 1)
				product_ingredient_quantities = product_ingredient_quantities + to_string(product_ingredient_quantity_temp) + ",";
			else
				product_ingredient_quantities += to_string(product_ingredient_quantity_temp);
		}
		cout << endl;
	}

	cout << "Product quantity: ";
	if (!dp::read_int(product_quantity))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	cout << "Product price: ";
	if (!dp::read_double(product_price))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (product_quantity > 0 && product_quantity < MAX_ITEM_QUANTITY)
	{
		if (handler->AddItem(id, product_type, product_name, product_ingredients, product_ingredient_quantities, to_string(product_quantity), to_string(product_price)))
		{
			Debug::Log("Successfully added new product \"" + product_name + "\" for machine #" + to_string(id) + ".", true);
			return true;
		}
		else
		{
			Debug::Log("Error while adding new product \"" + product_name + "\" for machine #" + to_string(id) + ".", true);
			return false;
		}
	}
	else
	{
		Debug::Log("Error while adding product. Product quantity should be >0 and <" + to_string(MAX_ITEM_QUANTITY) + ".", true);
		return false;
	}
}

bool Runtime::AddIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	string ingredient_name;
	int ingredient_quantity;

	Debug::Log("Adding new ingredients for machine #" + to_string(id) + ".");

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);
		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "=====================================" << endl
		<< "\tADD INGREDIENT:" << endl;

	cout << "Ingredient name: ";
	if (!dp::read_string(ingredient_name))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	ingredient_name = dp::remove_from(ingredient_name, ','); // Deleting delimiter characters from ingredient_name
	ingredient_name = dp::remove_from(ingredient_name, ';'); // if they were typed

	cout << "Ingredient quantity: ";
	if (!dp::read_int(ingredient_quantity))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
	{
		if (i->first == ingredient_name)
		{
			Debug::Log("Error while adding ingredient. Ingredient already exists.", true);
			return false;
		}
	}

	if (ingredient_quantity > 0 && ingredient_quantity < MAX_INGREDIENT_QUANTITY)
	{
		if (handler->AddIngredient(id, ingredient_name, to_string(ingredient_quantity)))
		{
			Debug::Log("Successfully added ingredient \"" + ingredient_name + "\" for machine #" + to_string(id) + ".", true);
			return true;
		}
		else
		{
			Debug::Log("Error while adding ingredient \"" + ingredient_name + "\" for machine #" + to_string(id) + ".", true);
			return false;
		}
	}
	else
	{
		cout << "Error while adding ingredient. Ingredient quantity should be >0 and <" << MAX_INGREDIENT_QUANTITY << "." << endl;
		return false;
	}
}

bool Runtime::RefillItem(int id, Machine* machine, ConnectionHandler* handler)
{
	vector<string> products;
	vector<string>::iterator iter;
	vector<string> indexes;
	string item_indexes;
	int index;
	string item;
	int quantity;

	Debug::Log("Refilling items for machine #" + to_string(id) + "...");

	products = handler->GetMachineItems(id);

	PrintProducts(machine);

	cout << "Item index or indexes (separated with ','): ";
	if (!dp::read_string(item_indexes))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	indexes = dp::split(dp::remove_from(item_indexes, ' '), ",");

	for (int i = 0; i < indexes.size(); i++)
	{
		Debug::Log("Refilling item #" + to_string(i + 1) + "...");

		try
		{
			index = stoi(indexes[i]);
		}
		catch (exception e)
		{
			Debug::Log(to_string(i + 1) + ". Index should be a number. You typed: " + indexes[i] + ".", true);
			continue;
		}

		if (index <= products.size() && index > 0)
		{
			iter = products.begin(); // Setting up iterator
			advance(iter, index - 1); // Decreasing iterator position by item_index picked - 1, as earlier it was increased

			item = machine->GetItemById(index - 1)->Get(); // Getting item name from machine by it's id

			cout << i + 1 << ". Add quantity of " << item << ": ";
			if (!dp::read_int(quantity)) // There may be some checking
			{
				Debug::Log("Input error, action aborted.", true);
				return false;
			}

			if (quantity > 0 && machine->GetItemById(index - 1)->GetQuantity() + quantity < MAX_ITEM_QUANTITY)
			{
				if (handler->TakeItem(id, item.c_str(), -quantity))
				{
					Debug::Log("Refilled item " + item + " with value " + to_string(quantity) + ".", true);
				}
				else
				{
					Debug::Log("Error while refilling item " + item + ".", true);
					continue;
				}
			}
			else
			{
				Debug::Log("Too high or too small quantity of " + item + " to be added. Minimum quantity allowed to be added is 1 and maximum is " + to_string(MAX_ITEM_QUANTITY - machine->GetItemById(index - 1)->GetQuantity() - 1) + ".", true);
				continue;
			}
		}
		else
		{
			Debug::Log(to_string(i + 1) + ". Item with index " + to_string(index) + " not found.", true);
			continue; // item index is a too big or a too small value
		}
	}

	Debug::Log("Successfully refilled items for machine #" + to_string(id) + ".");
	return true;
}

bool Runtime::RefillIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	map<string, int>::iterator iter;
	vector<string> indexes;
	string ingredient_indexes;
	int index;
	string ingredient;
	int quantity;

	Debug::Log("Refilling ingredients for machine #" + to_string(id) + "...");

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);
		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "Ingredient index or indexes (separated with ','): ";
	if (!dp::read_string(ingredient_indexes))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	indexes = dp::split(dp::remove_from(ingredient_indexes, ' '), ",");

	for (int i = 0; i < indexes.size(); i++)
	{
		Debug::Log("Refilling item #" + to_string(i + 1) + "...");

		try
		{
			index = stoi(indexes[i]);
		}
		catch (exception e)
		{
			Debug::Log(to_string(i + 1) + ". Index should be a number. You typed: " + indexes[i] + ".", true);
			continue;
		}

		if (index <= ingredients.size() && index > 0)
		{
			iter = ingredients.begin(); // Setting up iterator
			advance(iter, index - 1); // Decreasing iterator position by ingredient_index picked - 1, as earlier it was increased

			ingredient = iter->first; // Getting item name from machine by it's id

			cout << i + 1 << ". Add quantity of " << ingredient << ": ";
			if (!dp::read_int(quantity)) // There may be some checking
			{
				Debug::Log("Input error, action aborted.", true);
				return false;
			}

			if (quantity > 0 && ingredients[iter->first] + quantity < MAX_INGREDIENT_QUANTITY) // Check if ingredient quantity input is correct
			{
				if (handler->TakeIngredient(id, (iter->first).c_str(), -quantity))
				{
					Debug::Log("Refilled ingredient " + ingredient + " with value " + to_string(quantity) + ".", true);
				}
				else
				{
					Debug::Log("Error while refilling ingredient " + ingredient + ".", true);
					continue;
				}
			}
			else
			{
				Debug::Log("Too high or too small quantity of " + ingredient + " to be added. Minimum quantity allowed to be added is 1 and maximum is " + to_string(MAX_INGREDIENT_QUANTITY - iter->second - 1) + ".", true);
				continue;
			}
		}
		else
		{
			Debug::Log(to_string(i + 1) + ". Ingredient with index " + to_string(index) + " not found.", true);
			continue; // ingredient index is a too big or a too small value
		}
	}

	Debug::Log("Successfully refilled ingredients for machine #" + to_string(id) + ".");
	return true;
}

bool Runtime::RemoveProduct(int id, Machine* machine, ConnectionHandler* handler)
{
	int item_index = 0;

	Debug::Log("Removing item from machine #" + to_string(id) + "...");

	PrintProducts(machine);
	cout << endl;

	cout << "Item index: ";
	if (!dp::read_int(item_index))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (handler->DeleteItem(id, (machine->GetItemById(item_index - 1)->Get()).c_str())) // Decreasing iterator position by item_index picked - 1, as earlier it was increased
	{
		Debug::Log("Successfully removed item \"" + machine->GetItemById(item_index - 1)->Get() + "\" from machine #" + to_string(id) + ".", true);
		return true;
	}
	else
	{
		Debug::Log("Error while removing item \"" + machine->GetItemById(item_index - 1)->Get() + "\" from machine #" + to_string(id) + ".", true);
		return false;
	}
}

bool Runtime::RemoveIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	map<string, int>::iterator iter;
	int ingredient_index;

	Debug::Log("Removing ingredient from machine #" + to_string(id) + "...");

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);
		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "Ingredient index: ";
	if (!dp::read_int(ingredient_index))
	{
		Debug::Log("Input error, action aborted.", true);
		return false;
	}

	if (ingredient_index <= ingredients.size() && ingredient_index > 0)
	{
		iter = ingredients.begin(); // Setting up iterator
		advance(iter, ingredient_index - 1); // Decreasing iterator position by ingredient_index picked - 1, as earlier it was increased

		if (handler->DeleteIngredient(id, (iter->first).c_str()))
		{
			Debug::Log("Sucessfully removed ingredient \"" + iter->first + "\" from machine #" + to_string(id) + ".", true);
			return true;
		}
		else
		{
			Debug::Log("Error while removing ingredient \"" + iter->first + "\" from machine #" + to_string(id) + ".", true);
			return false;
		}
	}
	else
	{
		Debug::Log("Ingredient index was input incorrectly.", true);
		return false; // ingredient_index is a too big or a too small value
	}
}
