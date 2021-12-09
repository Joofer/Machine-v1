#include "libs\Runtime.h"

RuntimeListener::RuntimeListener()
{

}

//

void RuntimeListener::PrintConnect()
{
	cout << "=====================================" << endl
		<< "\t\tCONNECT TO DATABASE" << endl
		<< "" << endl
		<< "1\tConnect to database" << endl
		<< "" << endl
		<< "x\tExit" << endl
		<< "=====================================" << endl;
}

void RuntimeListener::PrintPick()
{
	cout << "=====================================" << endl
		<< "\t\tENTRY" << endl
		<< "" << endl
		<< "1\tConnect to an existing machine" << endl
		<< "2\tCreate a new machine" << endl
		<< "3\tDelete an existing machine" << endl
		<< "" << endl
		<< "x\tExit" << endl
		<< "=====================================" << endl;
}

bool RuntimeListener::Connect(ConnectionHandler* connection)
{
	string server;
	string user;
	string password;
	string database;

	cout << "Server: ";
	if (!dp::read_string(server))
		return false;

	cout << "User: ";
	if (!dp::read_string(user))
		return false;

	cout << "Password: ";
	if (!dp::read_string(password))
		return false;

	cout << "Database: ";
	if (!dp::read_string(database))
		return false;

	if (connection->ConnectToDatabase(server.c_str(), user.c_str(), password.c_str(), database.c_str())) // Connecting to database
	{
		return true;
	}
	else
	{
		cout << "Error while connecting to machine." << endl;
		return false;
	}
}

bool RuntimeListener::Pick(ConnectionHandler* connection, int& out_id)
{
	int id;

	cout << "Machine id: ";
	if (!dp::read_int(id))
		return false;

	if (connection->IsIdExisting(id))
	{
		out_id = id;
		return true;
	}
	else
	{
		cout << "Machine with ID " << id << " doesn't exist." << endl;
		return false;
	}
}

bool RuntimeListener::Create(ConnectionHandler* connection)
{
	int id;
	int temp_type;
	string name;
	string type;

	cout << "=====================================" << endl
		<< "\t\tCREATE NEW MACHINE" << endl
		<< "=====================================" << endl;

	cout << "Machine name: ";
	if (!dp::read_string(name))
		return false;

	cout << "Machine type (Beverage Machine - 1, Coffee Machine - 2, Wending Machine - 3): ";
	if (!dp::read_int(temp_type))
		return false;

	if (temp_type < 1 || temp_type > 3)
		return false;

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
		return -1;
	}

	if (connection->CreateMachine(name.c_str(), type.c_str(), id))
	{
		if (id != -1)
		{
			cout << "Machine created. Machine's id: " << id << "." << endl;

			return true;
		}
		else
		{
			cout << "id = -1" << endl;
			return false;
		}
	}
	else
	{
		cout << "on creating error" << endl;
		return false;
	}
}

bool RuntimeListener::Delete(ConnectionHandler* connection)
{
	int id;

	cout << "Machine id: ";
	if (!dp::read_int(id))
		return false;
	
	if (connection->IsIdExisting(id))
	{
		if (connection->DeleteMachine(id))
		{
			return true;
		}
		else
		{
			cout << "Error while deleting machine." << endl;

			return false;
		}
	}
	else
	{
		cout << "Machine with ID " << id << " doesn't exist." << endl;
		return false;
	}

}

//

int RuntimeListener::Buy(int id, Wallet* wallet, Machine* machine, string item, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	string prices;
	char price[255];

	// Date and time for logging

	char date_str[255];
	char time_str[255];
	
	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(date_str, sizeof(date_str), "%Y-%m-%d", &_time);
	strftime(time_str, sizeof(time_str), "%X", &_time);

	// cout << "Date: " << date_str << " time: " << time_str << "." << endl;

	//

	if (machine->FindId(item) != -1)
	{
		if (wallet->GetMoney() < machine->FindItem(item)->GetPrice())
		{
			return 1; // Not enough money
		}
		else
		{
			if (handler->TakeItem(id, item.c_str(), 1)) // We first check database and try to take an item from there
			{
				if (machine->Take(item))
				{
					ingredients = handler->GetNeededIngredients(id, item.c_str());

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

					for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
						if (!handler->TakeIngredient(id, (i->first).c_str(), i->second))
							cout << "Error while updating database with values: " << id << "\t" << i->first << "\t" << i->second << endl;

					
					if (!handler->LogPurchase(id, item.c_str(), prices.c_str(), machine->FindItem(item)->GetPrice(), date_str, time_str))
						cout << "Error while logging purchase." << endl;

					wallet->TakeMoney(machine->FindItem(item)->GetPrice());
					return -1;
				}
				else
				{
					handler->TakeItem(id, item.c_str(), -1);
					return 2; // Couldn't take an item from machine
				}
			}
			else
			{
				return 3; // Couldn't take an item from database
			}
		}
	}
	else
	{
		return 0;
	}
}

void RuntimeListener::ThrowError(Error error)
{
	cout << "Error: ";
	switch (error)
	{
	case Error::NOT_FOUND:
		cout << "Item not found." << endl;
		break;
	case Error::NO_MONEY:
		cout << "Not enough money." << endl;
		break;
	case Error::NO_QUANTITY:
		cout << "Not enough quantity." << endl;
		break;
	case Error::NO_CONNECTION:
		cout << "No connection with database or error while processing." << endl;
		break;
	case Error::ERROR_WHILE_MACHINE_CREATION:
		cout << "Error while creating a new machine." << endl;
		break;
	case Error::ERROR_WHILE_DELETING_MACHINE:
		cout << "Error while deleting a machine." << endl;
		break;
	case Error::ERROR_WHILE_LOGGING_PURCHASE:
		cout << "Error while logging purchase." << endl;
		break;
	default:
		cout << "Error while processing." << endl;
		break;
	}
}

void RuntimeListener::PrintName(Machine* machine)
{
	cout << "=====================================" << endl
		<< "CURRENT NAME: " << machine->GetName() << endl
		<< "=====================================" << endl;
}

void RuntimeListener::PrintMoney(Wallet* wallet)
{
	cout << "You have " << wallet->GetMoney() << endl;
}

void RuntimeListener::PrintSubtraction(double value)
{
	cout << " - " << value << endl;
}

void RuntimeListener::PrintMain()
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

void RuntimeListener::PrintCoffee(Machine* machine)
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

void RuntimeListener::PrintManage(int panel)
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

void RuntimeListener::PrintProducts(Machine* machine)
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

void RuntimeListener::PrintIngredients(Machine* machine)
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

bool RuntimeListener::SetName(int id, Machine* machine, ConnectionHandler* handler)
{
	string name;

	PrintName(machine);

	cout << "New name: ";
	if (!dp::read_string(name))
		return false;

	return handler->SetName(id, name.c_str());
}

bool RuntimeListener::AddProduct(int id, Machine* machine, ConnectionHandler* handler)
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

	cout << "=====================================" << endl
		<< "\tADD PRODUCT:" << endl;

	cout << "Product type (Beverage Machine - 1, Coffee Machine - 2, Wending Machine - 3): ";
	if (!dp::read_int(product_type_number))
		return false;

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
		cout << "Error: unknown product type." << endl;
		return false;
	}
	
	cout << "Product name (without spaces): ";
	if (!dp::read_string(product_name))
		return false;

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
			return false;

		PrintIngredients(machine);

		for (int i = 0; i < product_ingredients_count; i++)
		{
			cout << "Ingredient index " << i + 1 << ": ";
			if (!dp::read_int(ingredient_index))
				return false;

			if (ingredient_index <= ingredients.size() && ingredient_index > 0)
			{
				iter = ingredients.begin(); // Setting up iterator
				advance(iter, ingredient_index - 1); // Increasing iterator position by ingredient_index picked - 1, as earlier it was increased
				product_ingredient_temp = iter->first;
			}
			else
			{
				cout << "Error while picking ingredient." << endl;
				return false; // ingredient_index is a too big or a too small value
			}

			if (i != product_ingredients_count - 1)
				product_ingredients = product_ingredients + product_ingredient_temp + ",";
			else
				product_ingredients += product_ingredient_temp;

			cout << "Quantity: ";
			if (!dp::read_int(product_ingredient_quantity_temp))
				return false;

			if (product_ingredient_quantity_temp <= 0 || product_ingredient_quantity_temp >= MAX_INGREDIENT_QUANTITY) // Check if ingredient quantity input is correct
				return false;

			if (i != product_ingredients_count - 1)
				product_ingredient_quantities = product_ingredient_quantities + to_string(product_ingredient_quantity_temp) + ",";
			else
				product_ingredient_quantities += to_string(product_ingredient_quantity_temp);
		}
		cout << endl;
	}

	cout << "Product quantity: ";
	if (!dp::read_int(product_quantity))
		return false;

	cout << "Product price: ";
	if (!dp::read_double(product_price))
		return false;

	if (product_quantity > 0 && product_quantity < MAX_ITEM_QUANTITY)
	{
		return handler->AddItem(id, product_type, product_name, product_ingredients, product_ingredient_quantities, to_string(product_quantity), to_string(product_price));
	}
	else
	{
		cout << "Error while adding product. Product quantity should be >0 and <" << MAX_ITEM_QUANTITY << "." << endl;
		return false;
	}
}

bool RuntimeListener::AddIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	string ingredient_name;
	int ingredient_quantity;

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);

		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "=====================================" << endl
		<< "\tADD INGREDIENT:" << endl;

	cout << "Ingredient name (without spaces): ";
	if (!dp::read_string(ingredient_name))
		return false;

	cout << "Ingredient quantity: ";
	if (!dp::read_int(ingredient_quantity))
		return false;

	for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
	{
		if (i->first == ingredient_name)
		{
			cout << "Error while adding ingredient. Ingredient already exists." << endl;
			return false;
		}
	}

	if (ingredient_quantity > 0 && ingredient_quantity < MAX_INGREDIENT_QUANTITY)
	{
		return handler->AddIngredient(id, ingredient_name, to_string(ingredient_quantity));
	}
	else
	{
		cout << "Error while adding ingredient. Ingredient quantity should be >0 and <" << MAX_INGREDIENT_QUANTITY << "." << endl;
		return false;
	}
}

bool RuntimeListener::RefillItem(int id, Machine* machine, ConnectionHandler* handler)
{
	vector<string> products;
	vector<string>::iterator iter;
	string item;
	int quantity;
	int item_index;

	products = handler->GetProducts(id);

	PrintProducts(machine);

	cout << "Item index: ";
	if (!dp::read_int(item_index))
		return false;

	if (item_index <= products.size() && item_index > 0)
	{
		iter = products.begin(); // Setting up iterator
		advance(iter, item_index - 1); // Decreasing iterator position by item_index picked - 1, as earlier it was increased

		cout << "Quantity: ";
		if (!dp::read_int(quantity)) // There may be some checking
			return false;

		return handler->TakeItem(id, (machine->GetItemById(item_index - 1)->Get()).c_str(), -quantity);
	}
	else
	{
		return false; // item_index is a too big or a too small value
	}
}

bool RuntimeListener::RefillIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	map<string, int>::iterator iter;
	int ingredient_index;
	int ingredient_quantity;

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);

		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "Ingredient index: ";
	if (!dp::read_int(ingredient_index))
		return false;

	if (ingredient_index <= ingredients.size() && ingredient_index > 0)
	{
		iter = ingredients.begin(); // Setting up iterator
		advance(iter, ingredient_index - 1); // Decreasing iterator position by ingredient_index picked - 1, as earlier it was increased

		cout << "Quantity: ";
		if (!dp::read_int(ingredient_quantity)) // There may be some checking
			return false;

		if (ingredient_quantity > 0 && ingredients[iter->first] + ingredient_quantity < MAX_INGREDIENT_QUANTITY) // Check if ingredient quantity input is correct
			return handler->TakeIngredient(id, (iter->first).c_str(), -ingredient_quantity);
		else
			return false;
	}
	else
	{
		return false; // ingredient_index is a too big or a too small value
	}
}

bool RuntimeListener::RemoveProduct(int id, Machine* machine, ConnectionHandler* handler)
{
	int item_index = 0;

	PrintProducts(machine);
	cout << endl;

	cout << "Item index: ";
	if (!dp::read_int(item_index))
		return false;

	return handler->DeleteItem(id, (machine->GetItemById(item_index - 1)->Get()).c_str()); // Decreasing iterator position by item_index picked - 1, as earlier it was increased
}

bool RuntimeListener::RemoveIngredient(int id, Machine* machine, ConnectionHandler* handler)
{
	map<string, int> ingredients;
	map<string, int>::iterator iter;
	int ingredient_index;

	if (typeid(*machine) != typeid(CoffeeMachine))
	{
		ThrowError(Error::NO_CONNECTION);

		return false;
	}

	ingredients = dynamic_cast<CoffeeMachine*>(machine)->GetIngredients();

	PrintIngredients(machine);

	cout << "Ingredient index: ";
	if (!dp::read_int(ingredient_index))
		return false;

	if (ingredient_index <= ingredients.size() && ingredient_index > 0)
	{
		iter = ingredients.begin(); // Setting up iterator
		advance(iter, ingredient_index - 1); // Decreasing iterator position by ingredient_index picked - 1, as earlier it was increased

		return handler->DeleteIngredient(id, (iter->first).c_str());
	}
	else
	{
		return false; // ingredient_index is a too big or a too small value
	}
}
