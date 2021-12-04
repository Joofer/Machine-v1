#pragma once

#include <iostream>
#include <string>
#include <map>

using namespace std;

class MachineItem
{
public:
	map<string, int> ingredients_needed;
private:
	string item;
	int quantity;
	double price;
public:
	MachineItem();
	MachineItem(string name, int count, double value);
	MachineItem(string name, int count, double value, map<string, int>::iterator start, map<string, int>::iterator end);
	void Set(map<string, int>::iterator start, map<string, int>::iterator end);
	string GetIngredients();
	void SetQuantity(int count);
	void SetPrice(double value);
	void IncreaseQuantity(int count);
	void DecreaseQuantity();
	string Get();
	int GetQuantity();
	double GetPrice();
	bool Change(string name, int value);
	bool Delete(string name);
};