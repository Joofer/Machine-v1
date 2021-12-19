#include "libs/Debug.h"

Debug Debug::instance;

//

Debug::Debug()
{
	char log[255];

	sprintf_s(log, "log_%s-%s-%s.txt", Datetime::GetYear().c_str(), Datetime::GetMonth().c_str(), Datetime::GetDay().c_str());
	instance.filename = log;

	isLogging = false;
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

//

void Debug::Start()
{
	instance.isLogging = true;
}

void Debug::Stop()
{
	instance.isLogging = false;
}

//

void Debug::Log(string message, bool bPrintToConsole)
{
	char output[MAXCHARS];
	char time_str[255];

	// Opening file

	if (instance.isLogging)
	{
		if (!Open(instance.filename.c_str()))
			cout << "Error while opening log file." << endl;

		//

		sprintf_s(time_str, "%s:%s:%s", Datetime::GetHour().c_str(), Datetime::GetMin().c_str(), Datetime::GetSec().c_str());

		sprintf_s(output, "[%s]: %s\n", time_str, message.c_str());

		if (!instance.out_file.is_open())
		{
			cout << "Error while logging." << endl;
			return;
		}

		instance.out_file.write(reinterpret_cast<char*>(output), strlen(output));
	}

	if (bPrintToConsole)
		Print(message.c_str());

	// Closing file

	Close();
}

//

void Debug::Print(const char* message)
{
	cout << message << endl;
}
