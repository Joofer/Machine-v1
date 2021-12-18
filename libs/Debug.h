#pragma once

#include "Datetime.h"
#include <iostream>
#include <fstream>
#include <cstdarg>

using namespace std;

const int MAXCHARS = 1000;

class Debug
{
private:
	static Debug instance;
	ofstream out_file;
	string filename;

private:
	Debug();

public:
	static bool Open(const char* filename = "log.txt");
	static bool Close();
	
	static void Log(const char* message);
};