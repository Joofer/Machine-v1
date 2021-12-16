#include "libs/ConnectionHandler.h"

// Checking functions

bool ConnectionHandler::IsColumnExisting(const char* column, const char* table)
{
	char statement[MAXSTATEMENTCHARSLOWER];

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

	vector<string> data_products = GetMachineItems(id);
	vector<int> data_quantity = GetItemQuantities(id);
	vector<string> data_ingredients = GetMachineIngredients(id);
	vector<string> data_ingredients_quantity = GetMachineIngredientQuantities(id);
	vector<double> data_product_prices = GetItemPrices(id);

	if (data_products.size() != data_quantity.size() || data_quantity.size() != data_ingredients.size()
		|| data_ingredients.size() != data_ingredients_quantity.size() || data_ingredients_quantity.size() != data_product_prices.size())
		return false;

	// Ingredients row

	map<string, int> data_all_ingredients = GetAllIngredients(id);

	//

	return true;
}

// Processing functions

string ConnectionHandler::GetMachineName(int id)
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

string ConnectionHandler::GetMachineNames()
{
	string name;

	name = GetColumn("name");

	return name;
}

vector<string> ConnectionHandler::GetMachineItems(int id)
{
	vector<string> data;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "products");

		if (res.empty())
			return { };

		data = dp::split(res, ";");

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
		ingr = GetIngredients(id);
		quan = GetIngredientQuantities(id);

		if (ingr.empty() || quan.empty())
			return { };

		temp_ingredients = dp::split(ingr, ";");
		temp_quantity = dp::split(quan, ";");

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
		products = GetMachineItems(id);
		ingredients = GetMachineIngredients(id);
		quantities = GetMachineIngredientQuantities(id);

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
			result_ingredients = dp::split(ingredients[k], ",");
			result_quantities = dp::split(quantities[k], ",");

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

string ConnectionHandler::GetIngredients(int id)
{
	string ingredients;

	if (IsIdExisting(id))
	{
		ingredients = GetString(id, "ingredients", "ingredients");

		if (ingredients.empty())
			return "";

		return ingredients;
	}
	else
	{
		return "";
	}
}

string ConnectionHandler::GetIngredientQuantities(int id)
{
	string quantities;

	if (IsIdExisting(id))
	{
		quantities = GetString(id, "quantity", "ingredients");

		if (quantities.empty())
			return "";

		return quantities;
	}
	else
	{
		return "";
	}
}

vector<string> ConnectionHandler::GetMachineIngredients(int id)
{
	vector<string> ingredients;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "ingredients");

		if (res.empty())
			return { };

		ingredients = dp::split(res, ";");

		return ingredients;
	}
	else
	{
		return { };
	}
}

vector<string> ConnectionHandler::GetMachineIngredientQuantities(int id) // Get ingredients quantities
{
	vector<string> data;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "ingredients_quantity");

		if (res.empty())
			return { };

		data = dp::split(res, ";");

		return data;
	}
	else
	{
		return { };
	}
}

vector<map<string, int>> ConnectionHandler::GetMachineIngredientsVector(int id)
{
	vector<map<string, int>> data;
	map<string, int> temp;
	vector<string> ingredients;
	vector<string> quantities;
	vector<string> names_temp;
	vector<string> quantities_temp;

	if (IsIdExisting(id))
	{
		ingredients = GetMachineIngredients(id);
		quantities = GetMachineIngredientQuantities(id);

		if (ingredients.size() != quantities.size())
			return { };

		for (int i = 0; i < ingredients.size(); i++)
		{
			names_temp = dp::split(ingredients[i], ",");
			quantities_temp = dp::split(quantities[i], ",");

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

vector<int> ConnectionHandler::GetItemQuantities(int id)
{
	vector<string> data;
	vector<int> data_d;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "quantity");

		if (res.empty())
			return { };

		data = dp::split(res, ";");

		for (int i = 0; i < data.size(); i++)
			data_d.push_back(stoi(data[i]));

		return data_d;
	}
	else
	{
		return { };
	}
}

vector<double> ConnectionHandler::GetItemPrices(int id)
{
	vector<string> data;
	vector<double> data_d;
	string res;

	if (IsIdExisting(id))
	{
		res = GetString(id, "product_prices");

		if (res.empty())
			return { };

		data = dp::split(res, ";");

		for (int i = 0; i < data.size(); i++)
			data_d.push_back(stod(data[i]));

		return data_d;
	}
	else
	{
		return { };
	}
}

double ConnectionHandler::GetItemPrice(int id, string product)
{
	vector<string> products;
	vector<double> prices;

	if (IsIdExisting(id))
	{
		products = GetMachineItems(id);
		prices = GetItemPrices(id);

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
		data_ingredients = GetIngredients(id);
		data_quantities = GetIngredientQuantities(id);

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
		products = GetMachineItems(id);
		quantities = GetItemQuantities(id);

		for (int i = 0; i < products.size(); i++)
		{
			if (products[i] == name)
			{
				if (quantities[i] >= quantity)
				{
					quantities[i] -= quantity;
					q = dp::back_split(quantities, ";");
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
	vector<string> ingredients;
	vector<string> quantities;
	string res;

	if (IsIdExisting(id)) // Check if id is existing
	{
		ingredients = dp::split(GetIngredients(id), ";");
		quantities = dp::split(GetIngredientQuantities(id), ";");

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

		res = dp::back_split(quantities, ";");

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

		data_products = GetMachineItems(id);
		data_quantity = GetItemQuantities(id);
		data_ingredients = GetMachineIngredients(id);
		data_ingredients_quantity = GetMachineIngredientQuantities(id);
		data_product_prices = GetItemPrices(id);

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

		products = dp::back_split(data_products, ";");
		quantity = dp::back_split(data_quantity, ";");
		ingredients = dp::back_split(data_ingredients, ";");
		ingredients_quantity = dp::back_split(data_ingredients_quantity, ";");
		product_prices = dp::back_split(data_product_prices, ";");

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
		data_ingredients = dp::split(GetIngredients(id), ";");
		data_quantities = dp::split(GetIngredientQuantities(id), ";");

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

		ingredients = dp::back_split(data_ingredients, ";");
		quantities = dp::back_split(data_quantities, ";");

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