#include "libs/Machine.h"

Machine::Machine()
{
	name = "Unnamed Machine";
	items = { };
	money = 0;
}
Machine::~Machine()
{

}
void Machine::SetName(string n)
{
	name = n;
}
string Machine::GetName()
{
	return name;
}
void Machine::Add(MachineItem item)
{
	items.push_back(item);
}
void Machine::Add(vector<MachineItem>::iterator start, vector<MachineItem>::iterator end)
{
	items.insert(items.end(), start, end);
}
void Machine::SetIngredients(map<string, int> ingredients, bool is_reset)
{
	//
}
bool Machine::Take(string item)
{
	if (FindItem(item)->Get() != "N/A")
	{
		if (FindItem(item)->GetQuantity() >= 1)
		{
			FindItem(item)->DecreaseQuantity();
			money += FindItem(item)->GetPrice();
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
bool Machine::Delete(string item)
{
	if (FindId(item) != -1)
	{
		for (vector<MachineItem>::iterator i = items.begin(); i != items.end(); i++)
		{
			if (i->Get() == item)
			{
				items.erase(i);
				break;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool Machine::Change(string item, int value)
{
	if (FindId(item) != -1)
	{
		FindItem(item)->SetQuantity(value);
		return true;
	}
	else
	{
		return false;
	}
}
int Machine::FindId(string item)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].Get() == item)
		{
			return i;
		}
	}
	return -1;
}
int Machine::GetItemsCount()
{
	return items.size();
}
MachineItem* Machine::FindItem(string item)
{
	map<string, int> tmap = { };
	MachineItem* temp = new MachineItem("N/A", -1, -1);

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].Get() == item)
		{
			return &items[i];
		}
	}
	return temp;
}
MachineItem* Machine::GetItemById(int id)
{
	MachineItem* temp = new MachineItem("N/A", -1, -1);

	if (id <= items.size())
		if (items[id].Get() != "N/A")
			return &items[id];

	return temp;
}
string Machine::Check()
{
	for (vector<MachineItem>::iterator i = items.begin(); i != items.end(); i++)
	{
		if (i->GetQuantity() <= 0)
			return i->Get();
	}
	return "";
}

void Machine::ClearData()
{
	name = "Unnamed Machine";
	items = { };
	money = 0;
}
