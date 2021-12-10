#include "Connection.h"

// Constructors

Connection::Connection()
{
	driver = NULL;
	con = NULL;
	stmt = NULL;
	pstmt = NULL;
	result = NULL;
}

Connection::Connection(const char* server, const char* username, const char* password, const char* database)
{
	driver = NULL;
	con = NULL;
	stmt = NULL;
	pstmt = NULL;
	result = NULL;

	ConnectToDatabase(server, username, password, database); // We first null all the private variables and then try to connect
}

bool Connection::ConstructDataTable(bool checkPurchases)
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

// Connection functions

bool Connection::ConnectToDatabase(const char* server, const char* username, const char* password, const char* database)
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

bool Connection::Check()
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

bool Connection::Reconnect()
{
	uint32_t tick = 1000000;

	while (!con->reconnect())
		tick--;

	if (tick > 0)
		return true;
	else
		return false;
}

void Connection::Disconnect()
{
	delete pstmt;
	delete con;
}

// Private functions

string Connection::GetString(int id, const char* name, const char* table)
{
	char statement[255];
	string res;

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT %s FROM %s WHERE id = %d", name, table, id);

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

int Connection::GetInt(int id, const char* name, const char* table)
{
	char statement[255];
	int res;

	stmt = con->createStatement();
	sprintf_s(statement, "SELECT %s FROM %s WHERE id = %d", name, table, id);

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

bool Connection::UpdateString(uint64_t id, const char* name, const char* s, const char* table)
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

bool Connection::UpdateInt(uint64_t id, const char* name, int v, const char* table)
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

bool Connection::UpdateDouble(uint64_t id, const char* name, double v, const char* table)
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

bool Connection::UpdateUInt64(uint64_t id, const char* name, uint64_t v, const char* table)
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

bool Connection::UpdateRow(int id, const char* products, const char* quantity, const char* product_prices)
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

bool Connection::UpdateRow(int id, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices)
{
	if (!CheckDBStructure())
		return false;

	if (UpdateString(id, "products", products) && UpdateString(id, "quantity", quantity) && UpdateString(id, "ingredients", ingredients)
		&& UpdateString(id, "ingredients_quantity", ingredients_quantity) && UpdateString(id, "product_prices", product_prices))
		return true;

	cout << "Error while updating row. Additional manual data check may be needed. Full program reload is recommended." << endl;
	return false;
}

bool Connection::Insert(const char* name, const char* type, const char* products, const char* quantity, const char* ingredients, const char* ingredients_quantity, const char* product_prices, const char* ingredient_names, const char* ingredients_quantities, int& out_id)
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

bool Connection::Insert(int machine_id, const char* items, const char* prices, double total, const char* date, const char* time)
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

bool Connection::Delete(int id, const char* table)
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

bool Connection::CheckQuery(int id)
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

bool Connection::GetMaxId(int& out_id, const char* table)
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

bool Connection::GetMaxId(uint64_t& out_id, const char* table)
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
