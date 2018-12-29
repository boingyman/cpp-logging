#include "stdafx.h"
#include <chrono>
#include "logging.h"
//#include <filesystem>


//basic_log
const std::string bta::logging::basic_log::default_log_location(std::filesystem::current_path().string() + static_cast<char>(std::filesystem::path::preferred_separator) + "Log.txt");

bta::logging::basic_log::basic_log(const char* fileLocation) : _fileName(fileLocation), _file(std::ofstream(fileLocation, std::ios::out | std::ios::app)) { }

void bta::logging::basic_log::add_padding(int lines) {
	for (int i = 0; i < lines; ++i) {
		_file << std::endl;
	}
}

//default_log
bta::logging::default_log::default_log() : basic_log(default_log_location.c_str()) { }

bta::logging::default_log::default_log(const char* fileLocation) : basic_log(fileLocation) { }

bta::logging::basic_log* bta::logging::default_log::clone() const {
	return new default_log(_fileName);
}

void bta::logging::default_log::write(const char* content, int logLevel) {
	if (_file.bad() || _file.fail()) return;
	else {
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char time[32];
		int count = strftime(time, 32, "%c", localtime(&t));
		_file << "[" << std::string(time, count) << "](" << logLevel << ")  " << content << std::endl;
	}
}

//logger
bta::logging::logging_internal::logger bta::logging::logging_internal::logger::_current_logger = bta::logging::logging_internal::logger();

bta::logging::logging_internal::logger::logger() : _log(new default_log()), _loggingLevel(0) { }

bta::logging::logging_internal::logger::logger(basic_log* givenLog) : _log(givenLog), _loggingLevel(0) { }

bta::logging::logging_internal::logger::logger(const logger& copyFrom) : _log(copyFrom._log->clone()), _loggingLevel(copyFrom._loggingLevel) { }

bta::logging::logging_internal::logger::logger(logger&& moveFrom) : _log(moveFrom._log), _loggingLevel(moveFrom._loggingLevel) {
	moveFrom._log = nullptr;
	moveFrom._loggingLevel = 0;
}

bta::logging::logging_internal::logger::~logger() { 
	if (_log != nullptr) delete _log; 
}

bta::logging::logging_internal::logger& bta::logging::logging_internal::logger::operator=(bta::logging::logging_internal::logger& rhs) {
	this->_log = rhs._log->clone();
	this->_loggingLevel = rhs._loggingLevel;

	return *this;
}

bta::logging::logging_internal::logger& bta::logging::logging_internal::logger::operator=(bta::logging::logging_internal::logger&& rhs) {
	this->_log = rhs._log;
	rhs._log = nullptr;
	this->_loggingLevel = rhs._loggingLevel;
	rhs._loggingLevel = 0;

	return *this;
}

//bta::logging namespace members
void bta::logging::set_log_file(basic_log* logObject) {
	bta::logging::logging_internal::logger::_current_logger = std::move(bta::logging::logging_internal::logger(logObject));
}

void bta::logging::set_log_level(int level) {
	bta::logging::logging_internal::logger::_current_logger._loggingLevel = level;
}

int bta::logging::get_log_level() {
	return bta::logging::logging_internal::logger::_current_logger._loggingLevel;
}

void bta::logging::log_message(const char* message, int logLevel, int padding) {
	if (bta::logging::logging_internal::logger::_current_logger._log == nullptr) {
		bta::logging::logging_internal::logger::_current_logger._log = new default_log();
	}

	if (logLevel <= bta::logging::logging_internal::logger::_current_logger._loggingLevel) {
		bta::logging::logging_internal::logger::_current_logger._log->write(message, logLevel);
	}
}

void bta::logging::log_message(const std::string& message, int logLevel, int padding) {
	bta::logging::log_message(message.c_str(), logLevel);
}