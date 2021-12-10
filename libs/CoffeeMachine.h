#pragma once

#include "Machine.h"

class CoffeeMachine : public Machine
{
private:
	map<string, int> coffee_ingredients;
public:
	CoffeeMachine();
	void SetIngredients(map<string, int> ingredients, bool is_reset = false);
	bool Take(string item);
	bool AddIngredient(string ingredient, int value);
	bool TakeIngredient(string ingredient, int value);
	string Check(); // No fucking need to check items as IT WAS ALREADY CHECKED IN GET(FUCKING)QUANTITY FUNCTION, THAT'S GONNA BREAK THE ENTIRE PROGRAM!!!
	map<string, int> GetIngredients();
};