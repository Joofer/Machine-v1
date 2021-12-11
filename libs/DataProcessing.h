#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

class dp
{
public:
	//

	static vector<string> split(string s, string delimiter);
	static string back_split(vector<string> v, string delimiter);
	static string back_split(vector<int> v, string delimiter);
	static string back_split(vector<double> v, string delimiter);

	//

	static string remove_from(string s, char symbol);

	//

	static bool read_int(int& out);
	static bool read_double(double& out);
	static bool read_string(string& out);
};