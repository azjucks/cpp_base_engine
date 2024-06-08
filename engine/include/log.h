#pragma once

#include <singleton.h>
#include <string>

namespace Core
{
	namespace Debug
	{
		class Log : public Singleton<Log>
		{
			friend class Singleton<Log>;

		private:
			// Full text ready to be saved on drive
			std::string full_log;

		public:
			// Save logs on drive
			void save_logs(const std::string& filepath);

			// Main function for printing a log & adding it to our string
			void print_and_log(const std::string& str);

			// Formated logs for usual errors
			void error(const std::string& str);
			void cannot_open_file(const std::string& name);

			// Homemade formatted logs for usual things
			void start_load_obj(const std::string& filepath, const std::string& name);
			void success_load_obj(bool success);
			void start_load_scene(const std::string& filepath, const std::string& name);
			void success_load_scene(bool success);
			void saving_scene(const std::string& filepath, const std::string& name);
			void success_saving_scene(bool success);
			void creating_scene(const std::string& name);
			void closing_scene(const std::string& name);
		};
	}
}
