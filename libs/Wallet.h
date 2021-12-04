#pragma once

#include <iostream>
#include <string>

using namespace std;

class Wallet
{
private:
	double money;
	string holder;
public:
	Wallet();
	Wallet(double m, string h);
	void SetMoney(double value);
	double GetMoney();
	void TakeMoney(double value);
	void SetHolder(string name);
	const char* GetHolder();
};