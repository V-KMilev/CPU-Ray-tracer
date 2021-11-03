#pragma once

#include <fstream>

std::string fileToString(const char* file_name) {

	std::ifstream file(file_name);

	file.seekg(0, file.end);

	size_t file_size = file.tellg();

	std::string buffer;    // you can also buffer(file_size, ' ')
	buffer.resize(file_size);

	file.seekg(0, file.beg);

	file.read(&buffer[0], file_size);

	return buffer;
}
