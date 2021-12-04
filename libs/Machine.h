#pragma once

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "MachineItem.h"

using namespace std;

class Machine
{
private:

protected:
	string name;
	vector<MachineItem> items;
	double money;
public:
	Machine();
	~Machine();
	virtual void SetName(string name);
	virtual string GetName();
	virtual void Add(MachineItem item);
	virtual void Add(vector<MachineItem>::iterator start, vector<MachineItem>::iterator end);
	virtual void SetIngredients(map<string, int> ingredients, bool is_reset = false);
	virtual bool Take(string item);
	virtual bool Delete(string item);
	virtual bool Change(string item, int value);
	virtual int FindId(string item);
	virtual int GetItemsCount();
	virtual MachineItem* FindItem(string item);
	virtual MachineItem* GetItemById(int id);
	virtual string Check();
	virtual void ClearData();
};