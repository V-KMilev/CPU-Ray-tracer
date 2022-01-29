#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

class MyTOL {
	public:
		MyTOL() {}
		~MyTOL() {}

		bool myLoadObj(const char* filename, const char* basepath = NULL, bool triangulate = true) {
			std::cout << "Loading " << filename << std::endl;

			std::string warn;
			std::string err;
			bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, basepath, triangulate);

			if (!warn.empty()) {
				std::cout << "WARN: " << warn << std::endl;
			}

			if (!err.empty()) {
				std::cerr << "ERR: " << err << std::endl;
			}

			if (!ret) {
				std::cerr << "Failed to load/parse .obj.\n";
				return false;
			}

			return true;
		}

	public:
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
};
