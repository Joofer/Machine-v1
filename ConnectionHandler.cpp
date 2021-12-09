#include "libs/ConnectionHandler.h"

ConnectionHandler::ConnectionHandler()
{
	driver = NULL;
	con = NULL;
	stmt = NULL;
	pstmt = NULL;
	result = NULL;
}

ConnectionHandler::ConnectionHandler(const char* server, const char* username, const char* password, const char* database)
{
	driver = NULL;
	con = NULL;
	stmt = NULL;
	pstmt = NULL;
	result = NULL;

	ConnectToDatabase(server, username, password, database); // We first null all the private variables and then try to connect
}

bool ConnectionHandler::ConstructDataTable(bool checkPurchases)
{
	char statement_machine[] = "CREATE TABLE IF NOT EXISTS `data`( `id` INT NOT NULL AUTO_INCREMENT,  `name` varchar(255) NOT NULL, `type` VARCHAR(20) NOT NULL, `products` TEXT NOT NULL, `quantity` TEXT NOT NULL, `ingredients` TEXT NOT NULL, `ingredients_quantity` TEXT NOT NULL, `product_prices` TEXT NOT NULL, `is_on` TINYINT(1) DEFAULT NULL, PRIMARY KEY(`id`)) ENGINE = InnoDB AUTO_INCREMENT = 3 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci";
	char statement_ingredients[] = "CREATE TABLE IF NOT EXISTS `ingredients`( `id` INT NOT NULL AUTO_INCREMENT, `ingredients` TEXT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL, `quantity` TEXT NOT NULL, PRIMARY KEY(`id`)) ENGINE = InnoDB AUTO_INCREMENT = 6 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci";
	char statement_purchases[] = "CREATE TABLE `purchases` ( `id` bigint unsigned NOT NULL AUTO_INCREMENT, `machine_id` int NOT NULL, `items` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL, `prices` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL, `total` decimal(8, 2) NOT NULL, `date` date NOT NULL, `time` time NOT NULL, PRIMARY KEY(`id`)) ENGINE = InnoDB AUTO_INCREMENT = 2 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci";

	try
	{
		stmt = con->createStatement();
		stmt->execute(statement_machine);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	try
	{
		stmt = con->createStatement();
		stmt->execute(statement_ingredients);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	if (checkPurchases)
	{
		try
		{
			stmt = con->createStatement();
			stmt->execute(statement_purchases);
		}
		catch (sql::SQLException e)
		{
			// cout << "Error: " << e.what() << endl;
			return false;
		}
	}

	return true;
}

// Private functions

string ConnectionHandler::GetString(int id, const char* name)
{
	char statement[255];
	string res;

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT %s FROM data WHERE id = %d", name, id);

	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		exit(2);
	}

	if (!result->next())
	{
		cout << "Error while getting data from products." << endl;
		exit(2);
	}

	res = result->getString(1).c_str();

	delete result;

	return res;
}

int ConnectionHandler::GetInt(int id, const char* name)
{
	char statement[255];
	int res;

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT %s FROM data WHERE id = %d", name, id);

	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		exit(2);
	}

	if (!result->next())
	{
		cout << "Error while getting data from products." << endl;
		exit(2);
	}

	res = result->getInt(1);

	delete result;

	return res;
}

bool ConnectionHandler::UpdateString(uint64_t id, const char* name, const char* s, const char* table)
{
	char statement[16384];

	stmt = con->createStatement();
	sprintf_s(statement, "UPDATE %s SET %s = \"%s\" WHERE id = %llu", table, name, s, id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::UpdateInt(uint64_t id, const char* name, int v, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "UPDATE %s SET %s = %i WHERE id = %llu", table, name, v, id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::UpdateDouble(uint64_t id, const char* name, double v, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "UPDATE %s SET %s = %g WHERE id = %llu", table, name, v, id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::UpdateUInt64(uint64_t id, const char* name, uint64_t v, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "UPDATE %s SET %s = %llu WHERE id = %llu", table, name, v, id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::UpdateRow(int id, const char* products, const char* quantity, const char* product_prices)
{
	if (!CheckRowStructure(id))
		return false;

	if (!CheckDBStructure())
		return false;

	if (UpdateString(id, "products", products) && UpdateString(id, "quantity", quantity) && UpdateString(id, "product_prices", product_prices))
		return true;

	cout << "Error while updating row. Additional manual data check may be needed. Full program reload is recommended." << endl;
	return false;
}

bool ConnectionHandler::UpdateRow(int id, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices)
{
	if (!CheckDBStructure())
		return false;

	if (UpdateString(id, "products", products) && UpdateString(id, "quantity", quantity) && UpdateString(id, "ingredients", ingredients)
		&& UpdateString(id, "ingredients_quantity", ingredients_quantity) && UpdateString(id, "product_prices", product_prices))
		return true;

	cout << "Error while updating row. Additional manual data check may be needed. Full program reload is recommended." << endl;
	return false;
}

bool ConnectionHandler::Insert(const char* name, const char* type, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices, const char* ingredient_names, const char* ingredients_quantities, int& out_id)
{
	// Getting new id for the machine

	GetMaxId(out_id); // Get max query id
	out_id += 1; // Add 1 for the new machine's id

	// Create machine row

	char statement[1024];

	stmt = con->createStatement();
	sprintf_s(statement, "INSERT INTO data (`id`, `name`, `type`, `products`, `quantity`, `ingredients`, `ingredients_quantity`, `product_prices`, `is_on`) VALUES ('%d', '%s', '%s', '%s', '%s', '%s', '%s', '%s', 1)", out_id, name, type, products, quantity, ingredients, ingredients_quantity, product_prices);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	// Create ingredients row

	stmt = con->createStatement();
	sprintf_s(statement, "INSERT INTO ingredients (`id`, `ingredients`, `quantity`) VALUES ('%d', '', '')", out_id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::Insert(int machine_id, const char* items, const char* prices, double total, const char* date, const char* time)
{
	// Create machine row

	char statement[1024];

	stmt = con->createStatement();
	sprintf_s(statement, "INSERT INTO purchases (`machine_id`, `items`, `prices`, `total`, `date`, `time`) VALUES ('%i', '%s', '%s', '%g', '%s', '%s')", machine_id, items, prices, total, date, time);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::Delete(int id, const char* table)
{
	char statement[16384];

	stmt = con->createStatement();
	sprintf_s(statement, "DELETE FROM %s WHERE id = %d", table, id);

	try
	{
		stmt->execute(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::CheckQuery(int id)
{
	char statement[255];
	unsigned int count = 0;

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT * FROM data WHERE id = %d", id);

	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	if (!result->next())
	{
		cout << "Error while checking database query existance." << endl;
		return false;
	}
	else
	{
		count++;
	}

	while (result->next())
		count++;

	delete result;

	if (count > 0)
		return true;

	return false;
}

bool ConnectionHandler::GetMaxId(int& out_id, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT MAX(id) FROM %s", table);

	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	if (!result->next())
	{
		cout << "Error while getting data from products." << endl;
		return false;
	}

	out_id = result->getInt(1);

	delete result;

	return true;
}

bool ConnectionHandler::GetMaxId(uint64_t& out_id, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT MAX(id) FROM %s", table);

	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	if (!result->next())
	{
		cout << "Error while getting data from products." << endl;
		return false;
	}

	out_id = result->getUInt(1);

	delete result;

	return true;
}

//

bool ConnectionHandler::ConnectToDatabase(const char* server, const char* username, const char* password, const char* database)
{
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		con->setSchema(database);
	}
	catch (sql::SQLException e)
	{
		cout << "Could not connect to server. Error: " << e.what() << endl;
		exit(1);
	}

	if (!CheckDBStructure())
	{
		cout << "Error while checking database structure. Creating datatable..." << endl;
		
		if (!ConstructDataTable(true)) // In this case we use purchase logging so we need to set checkPurchases to true
		{
			cout << "Error while creating datatable. Additional manual data check and correction may be needed. Full program reload is recommended." << endl;

			return false;
		}
		else
		{
			cout << "Datatable created." << endl;

			return true;
		}
	}

	return true;
}

bool ConnectionHandler::Check()
{
	if (con->isValid())
	{
		if (Reconnect())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::Reconnect()
{
	uint32_t tick = 1000000;

	while (!con->reconnect())
		tick--;

	if (tick > 0)
		return true;
	else
		return false;
}

void ConnectionHandler::Disconnect()
{
	delete pstmt;
	delete con;
}

//

string ConnectionHandler::GetName(int id)
{
	string name;

	if (IsIdExisting(id))
	{
		name = GetString(id, "name");

		return name;
	}
	else
	{
		return "";
	}
}

vector<string> ConnectionHandler::GetProducts(int id)
{
	vector<string> data;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "products");

		if (res.empty())
			return { };

		data = split(res, ";");

		return data;
	}
	else
	{
		return { };
	}
}

map<string, int> ConnectionHandler::GetAllIngredients(int id)
{
	map<string, int> data;
	string ingr;
	string quan;
	vector<string> temp_ingredients;
	vector<string> temp_quantity;

	if (IsIdExisting(id))
	{
		pstmt = con->prepareStatement("SELECT ingredients, quantity FROM ingredients WHERE id = ?");
		pstmt->setInt(1, id);

		try
		{
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e)
		{
			// cout << "Error: " << e.what() << endl;
			return { };
		}

		if (!result->next())
		{
			cout << "Error while getting data from ingredients, quantity." << endl;
			exit(2);
		}

		ingr = result->getString(1).c_str();
		quan = result->getString(2).c_str();

		delete result;

		if (ingr.empty() || quan.empty())
			return { };

		temp_ingredients = split(ingr, ";");
		temp_quantity = split(quan, ";");

		if (temp_ingredients.size() != temp_quantity.size())
			return { };

		for (int i = 0; i < temp_ingredients.size(); i++)
			data.insert(make_pair(temp_ingredients[i], stoi(temp_quantity[i])));

		return data;
	}
	else
	{
		return { };
	}
}

map<string, int> ConnectionHandler::GetNeededIngredients(int id, const char* product)
{
	vector<string> products;
	vector<string> ingredients;
	vector<string> quantities;
	vector<string> result_ingredients;
	vector<string> result_quantities;
	map<string, int> res;
	unsigned int k = -1;

	if (IsIdExisting(id)) // Check if id is existing
	{
		products = GetProducts(id);
		ingredients = GetIngredients(id);
		quantities = GetQuantities(id);

		for (int i = 0; i < products.size(); i++)
		{
			if (products[i] == product)
			{
				k = i;
				break;
			}
		}

		if (k != -1)
		{
			result_ingredients = split(ingredients[k], ",");
			result_quantities = split(quantities[k], ",");

			for (int i = 0; i < result_ingredients.size(); i++)
				res.insert(make_pair(result_ingredients[i], stoi(result_quantities[i])));

			return res;
		}
		else
		{
			return { };
		}
	}
	else
	{
		return { };
	}
}

string ConnectionHandler::GetMachineIngredients(int id)
{
	string ingredients;

	if (IsIdExisting(id))
	{
		pstmt = con->prepareStatement("SELECT ingredients FROM ingredients WHERE id = ?");
		pstmt->setInt(1, id);

		try
		{
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e)
		{
			// cout << "Error: " << e.what() << endl;
			return { };
		}

		if (!result->next())
		{
			cout << "Error while getting data from ingredients table." << endl;
			exit(2);
		}

		ingredients = result->getString(1).c_str();

		delete result;

		if (ingredients.empty())
			return "";

		return ingredients;
	}
	else
	{
		return "";
	}
}

string ConnectionHandler::GetMachineIngredientsQuantities(int id)
{
	string quantities;

	if (IsIdExisting(id))
	{
		pstmt = con->prepareStatement("SELECT quantity FROM ingredients WHERE id = ?");
		pstmt->setInt(1, id);

		try
		{
			result = pstmt->executeQuery();
		}
		catch (sql::SQLException e)
		{
			// cout << "Error: " << e.what() << endl;
			return { };
		}

		if (!result->next())
		{
			cout << "Error while getting data from ingredients table." << endl;
			exit(2);
		}

		quantities = result->getString(1).c_str();

		delete result;

		if (quantities.empty())
			return "";

		return quantities;
	}
	else
	{
		return "";
	}
}

vector<string> ConnectionHandler::GetIngredients(int id)
{
	vector<string> data;
	vector<string> ingredients;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "ingredients");

		if (res.empty())
			return { };

		ingredients = split(res, ";");

		return ingredients;
	}
	else
	{
		return { };
	}
}

vector<string> ConnectionHandler::GetQuantities(int id) // Get ingredients quantities
{
	vector<string> data;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "ingredients_quantity");

		if (res.empty())
			return { };

		data = split(res, ";");

		return data;
	}
	else
	{
		return { };
	}
}

vector<map<string, int>> ConnectionHandler::GetIngredientsVector(int id)
{
	vector<map<string, int>> data;
	map<string, int> temp;
	vector<string> names;
	vector<string> quantities;
	vector<string> names_temp;
	vector<string> quantities_temp;

	if (IsIdExisting(id))
	{
		names = GetIngredients(id);
		quantities = GetQuantities(id);

		if (names.size() != quantities.size())
			return { };

		for (int i = 0; i < names.size(); i++)
		{
			names_temp = split(names[i], ",");
			quantities_temp = split(quantities[i], ",");

			if (names_temp.size() != quantities_temp.size())
				return { };

			for (int j = 0; j < names_temp.size(); j++)
				temp.insert(make_pair(names_temp[j], stoi(quantities_temp[j])));

			data.push_back(temp);

			temp.clear();
		}

		return data;
	}
	else
	{
		return { };
	}
}

vector<int> ConnectionHandler::GetQuantity(int id) // Get items quantities
{
	vector<string> data;
	vector<int> data_d;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "quantity");

		if (res.empty())
			return { };

		data = split(res, ";");

		for (int i = 0; i < data.size(); i++)
			data_d.push_back(stoi(data[i]));

		return data_d;
	}
	else
	{
		return { };
	}
}

vector<double> ConnectionHandler::GetPrices(int id)
{
	vector<string> data;
	vector<double> data_d;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "product_prices");

		if (res.empty())
			return { };

		data = split(res, ";");

		for (int i = 0; i < data.size(); i++)
			data_d.push_back(stod(data[i]));

		return data_d;
	}
	else
	{
		return { };
	}
}

double ConnectionHandler::GetPrice(int id, string product)
{
	vector<string> products;
	vector<double> prices;

	if (IsIdExisting(id))
	{
		products = GetProducts(id);
		prices = GetPrices(id);

		for (int i = 0; i < products.size(); i++)
			if (products[i] == product && prices[i] != NULL)
				return prices[i];

		return -1; // Product not found
	}
	else
	{
		return -1;
	}
}

bool ConnectionHandler::CreateMachine(const char* name, const char* type, int& out_id)
{
	if (Insert(name, type, "", "", "", "", "", "", "", out_id)) // We use empty template for the machine
		return true;
	else
		return false;
}

bool ConnectionHandler::DeleteMachine(int id)
{
	if (Delete(id))
	{
		if (Delete(id, "ingredients"))
		{
			return true;
		}
		else
		{
			// cout << "Error while deleting data from ingredients. Additional manual check may be required." << endl;
			return false;
		}
	}
	else
	{
		// cout << "Error while deleting data." << endl;
		return false;
	}
}

bool ConnectionHandler::SetName(int id, const char* name)
{
	if (IsIdExisting(id))
	{
		return UpdateString(id, "name", name);
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::AddItem(int id, string type, string name, string quantity, string price)
{
	string data_type;
	string data_products;
	string data_quantity;
	string data_product_prices;

	if (IsIdExisting(id))
	{
		data_type = GetString(id, "type");

		// IT CAN BE EMPTY
		// if (data_type.empty())
		// {
		// 	cout << "Error while getting data from Machine #" << id << "." << endl;
		// 	return false;
		// }

		data_products = GetString(id, "products");
		data_quantity = GetString(id, "quantity");
		data_product_prices = GetString(id, "product_prices");

		// IT CAN BE EMPTY
		// if (data_products.empty() || data_quantity.empty() || data_product_prices.empty())
		// {
		// 	cout << "Error while getting data from Machine #" << id << "." << endl;
		// 	return false;
		// }

		if (!data_products.empty())
		{
			data_products += ";" + name;
			data_quantity += ";" + quantity;
			data_product_prices += ";" + price;
		}
		else
		{
			data_products += name;
			data_quantity += quantity;
			data_product_prices += price;
		}

		if (!UpdateRow(id, data_products.c_str(), data_quantity.c_str(), data_product_prices.c_str()))
			return false;

		// Updating local variables should not be handled by this class
		//
		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::AddItem(int id, string type, string name, string ingredients, string ingredient_quantities, string quantity, string price)
{
	string data_type;
	string data_products;
	string data_quantity;
	string data_ingredients;
	string data_ingredients_quantity;
	string data_product_prices;

	if (IsIdExisting(id))
	{
		data_type = GetString(id, "type");

		// IT CAN BE EMPTY
		// if (data_type.empty())
		// {
		// 	cout << "Error while getting data from Machine #" << id << "." << endl;
		// 	return false;
		// }

		data_products = GetString(id, "products");
		data_quantity = GetString(id, "quantity");
		data_ingredients = GetString(id, "ingredients");
		data_ingredients_quantity = GetString(id, "ingredients_quantity");
		data_product_prices = GetString(id, "product_prices");

		// IT CAN BE EMPTY
		// if (data_products.empty() || data_quantity.empty() || data_ingredients.empty() || data_ingredients_quantity.empty() || data_product_prices.empty())
		// {
		// 	cout << "Error while getting data from Machine #" << id << "." << endl;
		// 	return false;
		// }

		if (!data_products.empty())
		{
			data_products += ";" + name;
			data_ingredients += ";" + ingredients;
			data_ingredients_quantity += ";" + ingredient_quantities;
			data_quantity += ";" + quantity;
			data_product_prices += ";" + price;
		}
		else
		{
			data_products += name;
			data_ingredients += ingredients;
			data_ingredients_quantity += ingredient_quantities;
			data_quantity += quantity;
			data_product_prices += price;
		}

		if (!UpdateRow(id, data_products.c_str(), data_quantity.c_str(), data_ingredients.c_str(), data_ingredients_quantity.c_str(), data_product_prices.c_str()))
			return false;

		// Updating local variables should not be handled by this class
		//
		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::AddIngredient(int id, string name, string quantity)
{
	string data_ingredients;
	string data_quantities;

	if (IsIdExisting(id))
	{
		data_ingredients = GetMachineIngredients(id);
		data_quantities = GetMachineIngredientsQuantities(id);

		// IT CAN BE EMPTY
		// if (data_ingredients.empty() || data_quantities.empty())
		// {
		// 	cout << "Error while getting data." << endl;
		// 	return false;
		// }

		if (!data_ingredients.empty())
		{
			data_ingredients += ";" + name;
			data_quantities += ";" + quantity;
		}
		else
		{
			data_ingredients += name;
			data_quantities += quantity;
		}

		if (!UpdateString(id, "ingredients", data_ingredients.c_str(), "ingredients"))
			return false;

		if (!UpdateString(id, "quantity", data_quantities.c_str(), "ingredients"))
			return false;

		// Updating local variables should not be handled by this class
		//
		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::TakeItem(int id, const char* name, int quantity)
{
	vector<string> products;
	vector<int> quantities;
	string q;

	if (IsIdExisting(id))
	{
		products = GetProducts(id);
		quantities = GetQuantity(id);

		for (int i = 0; i < products.size(); i++)
		{
			if (products[i] == name)
			{
				if (quantities[i] >= quantity)
				{
					quantities[i] -= quantity;
					q = back_split(quantities, ";");
					UpdateString(id, "quantity", q.c_str());
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::TakeIngredient(int id, const char* name, int quantity)
{
	// map<string, int> ingredients;
	vector<string> ingredients;
	vector<string> quantities;
	string res;

	if (IsIdExisting(id)) // Check if id is existing
	{
		ingredients = split(GetMachineIngredients(id), ";");
		quantities = split(GetMachineIngredientsQuantities(id), ";");

		if (ingredients.empty())
			return false;

		for (int i = 0; i < ingredients.size(); i++)
		{
			if (ingredients[i] == name)
			{
				quantities[i] = to_string(stoi(quantities[i]) - quantity);
				// cout << "---- Took " << quantity << " of " << ingredients[i] << " ended with quantity of " << quantities[i] << "." << endl;
			}
		}

		res = back_split(quantities, ";");

		UpdateString(id, "quantity", res.c_str(), "ingredients");

		// cout << "---- Took " << quantity << " of " << name << "." << endl;

		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::DeleteItem(int id, const char* name)
{
	string products;
	string quantity;
	string ingredients;
	string ingredients_quantity;
	string product_prices;
	string data_type;
	vector<string> data_products;
	vector<int> data_quantity;
	vector<string> data_ingredients;
	vector<string> data_ingredients_quantity;
	vector<double> data_product_prices;
	int pos = -1;

	if (IsIdExisting(id))
	{
		data_type = GetString(id, "type");

		if (data_type.empty())
		{
			cout << "Error while getting data from Machine #" << id << "." << endl;
			return false;
		}

		data_products = GetProducts(id);
		data_quantity = GetQuantity(id);
		data_ingredients = GetIngredients(id);
		data_ingredients_quantity = GetQuantities(id);
		data_product_prices = GetPrices(id);

		for (vector<string>::iterator i = data_products.begin(); i != data_products.end(); i++)
		{
			if (*i == name)
			{
				pos = distance(data_products.begin(), i);
				data_products.erase(i);
				break;
			}
		}

		if (pos < 0 || pos > data_products.size()) // We should add - 1 after data_products.size() but we have already deleted an element in for
		{
			cout << "Error while deleting data from Machine #" << id << "." << endl;
			return false;
		}

		data_quantity.erase(data_quantity.begin() + pos);
		data_ingredients.erase(data_ingredients.begin() + pos);
		data_ingredients_quantity.erase(data_ingredients_quantity.begin() + pos);
		data_product_prices.erase(data_product_prices.begin() + pos);

		products = back_split(data_products, ";");
		quantity = back_split(data_quantity, ";");
		ingredients = back_split(data_ingredients, ";");
		ingredients_quantity = back_split(data_ingredients_quantity, ";");
		product_prices = back_split(data_product_prices, ";");

		if (!UpdateRow(id, products.c_str(), quantity.c_str(), ingredients.c_str(), ingredients_quantity.c_str(), product_prices.c_str()))
			return false;

		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::DeleteIngredient(int id, const char* name)
{
	vector<string> data_ingredients;
	vector<string> data_quantities;
	string quantities;
	string ingredients;
	int pos = -1;

	if (IsIdExisting(id))
	{
		data_ingredients = split(GetMachineIngredients(id), ";");
		data_quantities = split(GetMachineIngredientsQuantities(id), ";");

		if (data_ingredients.empty() || data_quantities.empty())
		{
			cout << "Error while getting data." << endl;
			return false;
		}

		for (vector<string>::iterator i = data_ingredients.begin(); i != data_ingredients.end(); i++)
		{
			if (*i == name)
			{
				pos = distance(data_ingredients.begin(), i);
				data_ingredients.erase(i);
				break;
			}
		}

		if (pos < 0 || pos > data_ingredients.size()) // We should add - 1 after data_products.size() but we have already deleted an element in for
		{
			cout << "Error while deleting data." << endl;
			return false;
		}

		data_quantities.erase(data_quantities.begin() + pos);

		ingredients = back_split(data_ingredients, ";");
		quantities = back_split(data_quantities, ";");

		if (!UpdateString(id, "ingredients", ingredients.c_str(), "ingredients"))
			return false;

		if (!UpdateString(id, "quantity", quantities.c_str(), "ingredients"))
			return false;

		return true;
	}
	else
	{
		return false;
	}
}

bool ConnectionHandler::LogPurchase(int id, const char* items, const char* prices, double total, const char* date, const char* time)
{
	if (!Insert(id, items, prices, total, date, time))
		return false;

	return true;
}

//

bool ConnectionHandler::IsColumnExisting(const char* column, const char* table)
{
	char statement[255];

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT %s FROM %s", column, table);
	
	try
	{
		result = stmt->executeQuery(statement);
	}
	catch (sql::SQLException e)
	{
		// cout << "Error: " << e.what() << endl;
		return false;
	}

	if (!result->next())
	{
		// cout << "Error while checking column." << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool ConnectionHandler::IsDataTableExisting(const char* table, vector<const char*> columns)
{
	for (int i = 0; i < columns.size(); i++)
	{
		if (!IsColumnExisting(columns[i], table))
		{
			cout << "Error while checking datatable. Column " << columns[i] << " does not exist." << endl;
			return false;
		}
	}

	return true;
}

bool ConnectionHandler::IsIdExisting(int id)
{
	if (CheckQuery(id)) return true;

	return false;
}

bool ConnectionHandler::CheckDBStructure(bool checkPurchases)
{
	if (!IsDataTableExisting("data",
		{
			"id",
			"name",
			"type",
			"products",
			"quantity",
			"ingredients",
			"ingredients_quantity",
			"product_prices",
			"is_on"
		})
		) return false;

	if (!IsDataTableExisting("ingredients",
		{
			"id",
			"ingredients",
			"quantity"
		})
		) return false;

	if (checkPurchases)
		if (!IsDataTableExisting("purchases",
			{
				"id",
				"machine_id",
				"items",
				"prices",
				"total",
				"date",
				"time"
			})
			) return false;

	return true;
}

bool ConnectionHandler::CheckRowStructure(int id)
{
	if (!IsIdExisting(id))
		return false;

	// Machine row
	
	vector<string> data_products = GetProducts(id);
	vector<int> data_quantity = GetQuantity(id);
	vector<string> data_ingredients = GetIngredients(id);
	vector<string> data_ingredients_quantity = GetQuantities(id);
	vector<double> data_product_prices = GetPrices(id);

	if (data_products.size() != data_quantity.size() || data_quantity.size() != data_ingredients.size()
		|| data_ingredients.size() != data_ingredients_quantity.size() || data_ingredients_quantity.size() != data_product_prices.size())
		return false;

	// Ingredients row

	map<string, int> data_all_ingredients = GetAllIngredients(id);

	//

	return true;
}

//

vector<string> ConnectionHandler::split(string s, string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

string ConnectionHandler::back_split(vector<string> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += v[k];
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

string ConnectionHandler::back_split(vector<int> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += to_string(v[k]);
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

string ConnectionHandler::back_split(vector<double> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += to_string(v[k]);
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

