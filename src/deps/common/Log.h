#pragma once

#include <iostream>
#include <sstream>
#include <string>

class Logger {
	public:
		~Logger() { 
			delete log;
			log = nullptr;
		}

		/*
		 * Function - getDefaultLogger
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * std::stringstream&
		 * 
		 * Use:
		 * This funtion is funtion to the class. 
		 * By calling it you ask for the in return log. 
		 * The log is std::stringstream.
		 */

		static std::stringstream& getDefaultLogger() {
			if (log == nullptr) {
				log = new std::stringstream();
			}

			return *log;
		}

		/*
		 * Function - get
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * std::string
		 * 
		 * Use:
		 * By calling get, you ask for 
		 * the log content in return. 
		 * In debug get returns empty string.
		 */

		std::string get() {
			std::string res = (*log).str();

			if(res.length() > 10000) {
				(*log).str(std::string());
			}

			if(debug) { return ""; }

			return res;
		}

	private:
		/* We never create object so we don't need constructor */
		Logger() {}

	private:
		#ifdef DEBUG
			bool debug = true;
		#else
			bool debug = false;
		#endif

		static std::stringstream *log;
};

/* TODO: add levles of logs */