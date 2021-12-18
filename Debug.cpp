#include "libs/Debug.h"

Debug Debug::instance;

//

Debug::Debug()
{
	char log[255];

	sprintf_s(log, "log_%s-%s-%s.txt", Datetime::GetYear().c_str(), Datetime::GetMonth().c_str(), Datetime::GetDay().c_str());
	instance.filename = log;
}

bool Debug::Open(const char* filename)
{
	instance.out_file.open(filename, ios::out | ios::app);

	if (!instance.out_file.is_open())
		return false;

	return true;
}

bool Debug::Close()
{
	instance.out_file.close();

	return true;
}

void Debug::Log(const char* message)
{
	char output[MAXCHARS];
	char date_str[255];
	char time_str[255];

	// Opening file

	if (!Open(instance.filename.c_str()))
		cout << "Error while opening log file." << endl;

	//

	sprintf_s(date_str, "%s-%s-%s", Datetime::GetYear().c_str(), Datetime::GetMonth().c_str(), Datetime::GetDay().c_str());
	sprintf_s(time_str, "%s:%s:%s", Datetime::GetHour().c_str(), Datetime::GetMin().c_str(), Datetime::GetSec().c_str());

	sprintf_s(output, "[%s | %s]: %s\n", date_str, time_str, message);

	if (!instance.out_file.is_open())
	{
		cout << "Error while logging." << endl;
		return;
	}

	instance.out_file.write(reinterpret_cast<char*>(output), strlen(output));

	// Closing file

	Close();
}
