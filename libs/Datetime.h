#pragma once

#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

const int MAXTIMECHARS = 10;

class Datetime
{
public:
	static string GetSec();
	static string GetMin();
	static string GetHour();
	static string GetDay();
	static string GetMonth();
	static string GetYear();
};