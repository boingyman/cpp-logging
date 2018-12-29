#pragma once

#ifndef __BTA_LOGGING__
#define __BTA_LOGGING__

#include <string>
#include <fstream>
#include <filesystem>

namespace bta {
	namespace logging {
		class basic_log;

		//namespace functions for logging
		void set_log_file(basic_log* filePath);
		void set_log_level(int level);
		int get_log_level();
		void log_message(const char* message, int logLevel = 0, int padding = 0);
		void log_message(const std::string& message, int logLevel = 0, int padding = 0);

		//interface to inherit from for logging purposes
		class basic_log {
		protected:
			std::ofstream _file;
			const char* _fileName;
			int _loggingLevel;

		public:
			static const std::string default_log_location;
			
			basic_log(const char* fileLocation);
			void add_padding(int lines);
			virtual void write(const char* content, int logLevel) = 0;
			virtual basic_log* clone() const = 0;
		};


		//a default class for out of the box, simple use of logging
		class default_log : public basic_log {
		public:
			default_log();
			default_log(const char* fileLocation);
			void write(const char* content, int logLevel) override;
			basic_log* clone() const override;
		};

		//internal stuff, not meant to see the light of day
		namespace logging_internal {
			class logger final {
				static logger _current_logger;

				basic_log* _log;
				int _loggingLevel;

				logger();
				logger(basic_log* givenLog);
				logger(const logger& copyFrom);
				logger(logger&& moveFrom);
				~logger();

				logger& operator=(logger& rhs);
				logger& operator=(logger&& rhs);

				friend void bta::logging::set_log_file(basic_log*);
				friend void bta::logging::set_log_level(int);
				friend int bta::logging::get_log_level();
				friend void bta::logging::log_message(const char*, int, int);
			public:
			};
		}
	}
}


//convenience macros
#define LOG_MSG bta::logging::log_message
#define LOG_LEVEL_GET bta::logging::get_log_level
#define LOG_LEVEL_SET bta::logging::set_log_level
#define LOG_SET bta::logging::set_log_file

#endif