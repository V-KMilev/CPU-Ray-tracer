#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

class MyTOL {
	public:
		MyTOL() {}
		~MyTOL() {}

		/*
		 * Function - myLoadObj
		 *
		 * Parameters:
		 * [p] const char* file_name,
		 * [p] const char* file_name (by defult equals to NULL),
		 * [p] bool triangulate (by defult equals to true)
		 * 
		 * Return type:
		 * bool
		 * 
		 * Use:
		 * By giving file name (file path) you set
		 * object file to be read. 
		 * If the load failed in return you will get 1.
		 */

		bool myLoadObj(const char* file_name, const char* material_path = NULL, bool triangulate = true) {
			std::cout << "[OBJ][LOADING] myLoadObj - [ " << file_name << " ]\n";

			std::string warn;
			std::string err;
			bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_name, material_path, triangulate);

			if (!warn.empty()) {
				std::cout << "[OBJ][WARRNING] myLoadObj - [ " << warn << " ]\n";
			}

			if (!err.empty()) {
				std::cerr << "[OBJ][ERROR] myLoadObj - [" << err << " ]\n";
			}

			if (!ret) {
				std::cerr << "[OBJ][FAIL] myLoadObj - [ Failed to load/parse .obj. ]\n";
				return false;
			}

			return true;
		}

	public:
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
};
