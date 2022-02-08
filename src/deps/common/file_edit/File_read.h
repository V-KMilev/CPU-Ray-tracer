#pragma once

#include <fstream>

/*
 * Function - fileToString
 *
 * Parameters:
 * [p] const std::string& file_name
 * 
 * Return type:
 * const std:string
 * 
 * Use:
 * By giving a file name (file path) to the 
 * funtion you, get in return the content 
 * of the file in string form.
 */

const std::string fileToString(const std::string &file_name) {

	FILE *file = fopen(&file_name[0], "r");

	fseek(file, 0, SEEK_END);

	size_t file_size = ftell(file);

	fseek(file, 0, SEEK_SET);

	std::string buffer;    // you can also use buffer(file_size, ' ')
	buffer.resize(file_size);

	fread(&buffer[0], 1, file_size, file);
	fclose(file);

	return buffer;
}
