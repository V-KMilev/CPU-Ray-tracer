#pragma once

#include <iostream>
#include <sstream>
#include <string>

class Logger {
	public:
		~Logger() { 
			delete lg;
			lg = nullptr;
		}

		static std::stringstream& getDefaultLogger() {
			if (lg == nullptr) {
				lg = new std::stringstream();
			}

			return *lg;
		}

		std::string get() {
			std::string res = (*lg).str();

			if(res.length() > 10000) {
				(*lg).clear();
			}

			if(debug) { return ""; }

			return res;
		}

	private:
		Logger() {}

	private:
		#ifdef DEBUG
			bool debug = true;
		#else
			bool debug = false;
		#endif

		static std::stringstream *lg;
};

enum LogLevel {
	BASIC = 0,
	WARNING = 1,
	ERROR = 2
};

/* TODO: add levles of logs */