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

		static std::stringstream& getDefaultLogger() {
			if (log == nullptr) {
				log = new std::stringstream();
			}

			return *log;
		}

		std::string get() {
			std::string res = (*log).str();

			if(res.length() > 10000) {
				(*log).str(std::string());
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

		static std::stringstream *log;
};

/* TODO: add levles of logs */