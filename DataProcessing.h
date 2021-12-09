#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

class DataProcessing
{
public:
	//

	static vector<string> split(string s, string delimiter);
	static string back_split(vector<string> v, string delimiter);
	static string back_split(vector<int> v, string delimiter);
	static string back_split(vector<double> v, string delimiter);

	//

	static bool read_int(int& out);
	static bool read_double(double& out);
	static bool read_string(string& out);
};