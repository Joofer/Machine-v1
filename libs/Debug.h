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
	bool isLogging;

private:
	Debug();

public:
	static bool Open(const char* filename = "log.txt");
	static bool Close();

	static void Start(); // Start logging messages
	static void Stop(); // Stop logging messages

	static void Log(string message, bool bPrintToConsole = false);

	static void Print(const char* message); // Print log message to console, called even if logging is disabled
};