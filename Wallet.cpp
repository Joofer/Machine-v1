#include "libs/Wallet.h"

Wallet::Wallet()
{
	money = 0;
	holder = "N/A";
}

Wallet::Wallet(double m, string h)
{
	money = m;
	holder = h;
}

void Wallet::SetMoney(double value)
{
	money = value;
}

double Wallet::GetMoney()
{
	return money;
}

void Wallet::TakeMoney(double value)
{
	money -= value;
}

void Wallet::SetHolder(string name)
{
	holder = name;
}

const char* Wallet::GetHolder()
{
	return holder.c_str();
}
