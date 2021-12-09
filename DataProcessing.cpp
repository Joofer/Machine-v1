#include "DataProcessing.h"

//

vector<string> DataProcessing::split(string s, string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

string DataProcessing::back_split(vector<string> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += v[k];
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

string DataProcessing::back_split(vector<int> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += to_string(v[k]);
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

string DataProcessing::back_split(vector<double> v, string delimiter)
{
	string s = "";
	size_t k = 0;

	while (k < v.size())
	{
		s += to_string(v[k]);
		if (k < v.size() - 1) s += ";";
		k++;
	}

	return s;
}

//

bool DataProcessing::read_int(int& out)
{
	int i;

	cin >> i;

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		return false;
	}

	out = i;

	return true;
}

bool DataProcessing::read_double(double& out)
{
	double i;

	cin >> i;

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		return false;
	}

	out = i;

	return true;
}

bool DataProcessing::read_string(string& out)
{
	string s;

	cin.ignore(INT_MAX, '\n');
	getline(cin, s);

	if (!cin)
	{
		cin.clear();

		return false;
	}

	out = s;

	if (s != "-1") // For in-creation and in-edititing runtime exit
		return true;
	else
		return false;
}