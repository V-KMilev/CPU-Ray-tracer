#pragma once

#include <fstream>

const std::string fileToString(const std::string &file_name) {

	FILE *file = fopen(&file_name[0], "r");

	fseek(file, 0, SEEK_END);
	
	size_t file_size = ftell(file);
	
	fseek(file, 0, SEEK_SET);

	std::string buffer;    // you can also buffer(file_size, ' ')
	buffer.resize(file_size);

	fread(&buffer[0], 1, file_size, file);
	fclose(file);

	return buffer;
}
