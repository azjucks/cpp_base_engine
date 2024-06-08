#include <log.h>
using namespace Core::Debug;

#include <iostream>

#include <fstream>
#include <sstream>

#include <ctime>


const std::string currentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	localtime_s(&tstruct, &now);
	char buf[80];
	strftime(buf, sizeof(buf), "[%Y_%m_%d][%H-%M-%S]", &tstruct);

	return buf;
}

const std::string currentTime()
{
	time_t now = time(0);
	struct tm tstruct;
	localtime_s(&tstruct, &now);
	char buf[80];
	strftime(buf, sizeof(buf), "[%H:%M:%S]", &tstruct);

	return buf;
}

void Log::print_and_log(const std::string& str)
{
	std::string time = currentTime();
	std::cout << time + ' ' + str << std::endl;
	full_log += time + ' ' + str + "\n";
}

void Log::save_logs(const std::string& filepath)
{
	std::string name = "Logs_" + currentDateTime() + ".txt";
	std::ofstream file(filepath + name);
	if (!file.good())
	{
		cannot_open_file(filepath + name);
		return;
	}

	file << full_log;

	file.close();
}

void Log::error(const std::string& str)
{
	print_and_log("/!\\ " + str);
}

void Log::cannot_open_file(const std::string& name)
{
	print_and_log("/!\\ Can't open file : " + name);
	print_and_log("(Please check filepath & file integrity)");
}

void Log::start_load_obj(const std::string& filepath, const std::string& name)
{
	print_and_log("------------------------");
	print_and_log("Starting load of object : " + name + " ...");
	print_and_log("(File location : " + filepath + ").");
}
void Log::success_load_obj(bool success)
{
	std::string res = (success ? "Success!" : "Failure");
	print_and_log("Loading object : " + res);
	print_and_log("------------------------");
}

void Log::start_load_scene(const std::string& filepath, const std::string& name)
{
	print_and_log("========================");
	print_and_log("Starting load of scene : " + name + " ...");
	print_and_log("(File location : " + filepath + ").");
}
void Log::success_load_scene(bool success)
{
	std::string res = (success ? "Success!" : "Failure");
	print_and_log("Loading scene : " + res);
	print_and_log("========================");
}

void Log::saving_scene(const std::string& filepath, const std::string& name)
{
	print_and_log("Saving scene : " + name + " ...");
	print_and_log("(File location : " + filepath + ").");
}
void Log::success_saving_scene(bool success)
{
	std::string res = (success ? "Success!" : "Failure");
	print_and_log("Saving scene : " + res);
}

void Log::creating_scene(const std::string& name)
{
	print_and_log("-> New scene created, named : " + name);
}
void Log::closing_scene(const std::string& name)
{
	print_and_log("->  Scene " + name + " closed.");
}

