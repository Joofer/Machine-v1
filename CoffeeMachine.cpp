#include "libs/CoffeeMachine.h"

CoffeeMachine::CoffeeMachine()
{
	coffee_ingredients = {
	{"coffee_beans", 0},
	{"milk", 0},
	{"sugar", 0},
	{"water", 0}
	};
}
void CoffeeMachine::SetIngredients(map<string, int> ingredients, bool is_reset)
{
	if (is_reset)
		coffee_ingredients.clear();

	for (map<string, int>::iterator i = ingredients.begin(); i != ingredients.end(); i++)
		coffee_ingredients.insert(make_pair(i->first, i->second));
}
bool CoffeeMachine::Take(string item)
{
	if (FindItem(item)->Get() != "N/A")
	{
		// cout << "Item isnt N/A" << endl;
		if (FindItem(item)->GetQuantity() >= 1)
		{
			// cout << "Item quantity >= 1" << endl;
			if (Check(item) == "")
			{
				// cout << "Ingredients are in enough quantity" << endl;
				for (map<string, int>::iterator i = FindItem(item)->ingredients_needed.begin(); i != FindItem(item)->ingredients_needed.end(); i++)
				{
					coffee_ingredients[i->first] -= i->second; // Add check if ingredient add is not in list
					// cout << "Ingredient " << i->first << " decreased. Left " << coffee_ingredients[i->first] << endl;
				}
				FindItem(item)->DecreaseQuantity();
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
	else
	{
		return false;
	}
}
bool CoffeeMachine::AddIngredient(string ingredient, int value)
{
	if (coffee_ingredients.count(ingredient))
	{
		coffee_ingredients[ingredient] += value;
		return true;
	}
	else
	{
		return false;
	}
}
bool CoffeeMachine::TakeIngredient(string ingredient, int value)
{
	if (coffee_ingredients.count(ingredient))
	{
		if (coffee_ingredients[ingredient] >= value)
		{
			coffee_ingredients[ingredient] -= value;
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
string CoffeeMachine::Check(string item)
{
	if (item.empty())
	{
		for (map<string, int>::iterator i = coffee_ingredients.begin(); i != coffee_ingredients.end(); i++)
		{
			if (i->second <= 0)
			{
				return i->first;
			}
		}
	}
	else
	{
		if (FindItem(item)->Get() != "N/A") // Checking if item exists
		{
			for (map<string, int>::iterator i = FindItem(item)->ingredients_needed.begin(); i != FindItem(item)->ingredients_needed.end(); i++)
			{
				if (coffee_ingredients.contains(i->first))
				{
					if (coffee_ingredients[i->first] < i->second)
					{
						return i->first;
					}
				}
				else
				{
					return "N/F";
				}
			}
		}
		else
		{
			return "N/F";
		}
	}
	return "";
}

map<string, int> CoffeeMachine::GetIngredients()
{
	return coffee_ingredients;
}
