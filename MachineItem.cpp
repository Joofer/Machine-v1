#include "libs/MachineItem.h"

MachineItem::MachineItem()
{
	ingredients_needed = { };
}

MachineItem::MachineItem(string name, int count, double value)
{
	item = name;
	quantity = count;
	price = value;
}

MachineItem::MachineItem(string name, int count, double value, map<string, int>::iterator start, map<string, int>::iterator end)
{
	item = name;
	quantity = count;
	price = value;
	ingredients_needed.insert(start, end);
}

void MachineItem::Set(map<string, int>::iterator start, map<string, int>::iterator end)
{
	ingredients_needed.insert(start, end);
}

string MachineItem::GetIngredients()
{
	string ingredients = "";
	int k = 0;

	for (map<string, int>::iterator i = ingredients_needed.begin(); i != ingredients_needed.end(); i++)
	{
		ingredients += i->first + ":" + to_string(i->second);

		if (k != ingredients_needed.size() - 1) ingredients += ", ";

		k++;
	}

	return ingredients;
}

void MachineItem::SetQuantity(int count)
{
	quantity = count;
}

void MachineItem::SetPrice(double value)
{
	price = value;
}

void MachineItem::IncreaseQuantity(int count)
{
	quantity += count;
}

void MachineItem::DecreaseQuantity()
{
	quantity -= 1;
}

string MachineItem::Get()
{
	return item;
}

int MachineItem::GetQuantity()
{
	return quantity;
}

double MachineItem::GetPrice()
{
	return price;
}

bool MachineItem::Change(string name, int value)
{
	if (ingredients_needed.count(name))
	{
		ingredients_needed[name] = value;
		return true;
	}
	else
	{
		return false;
	}
}

bool MachineItem::Delete(string name)
{
	if (ingredients_needed.count(name))
	{
		ingredients_needed.erase(name);
		return true;
	}
	else
	{
		return false;
	}
}
