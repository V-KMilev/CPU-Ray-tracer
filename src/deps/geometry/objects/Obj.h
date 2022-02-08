#pragma once

#include "RT_tiny_obj_loader.h"

#include "Settings.h"
#include "Material.h"

// TODO: FIX materials

class obj : public Hittable {
	public:
		obj(MyEmbree &embree, const char* path, const char* material_path, shared_ptr<Material> material)
			: embree(embree), filePath(path), basePath(basePath), material_ptr(material)
		{
			myTOL.myLoadObj(path, material_path);

			embree.setup(RTC_GEOMETRY_TYPE_TRIANGLE);

			size_t v_size = myTOL.attrib.vertices.size();

			float* vertexbuffer = (float*) rtcSetNewGeometryBuffer(
					embree.geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), v_size);

			for(int i = 0; i < v_size; i++) {
				vertexbuffer[i] = myTOL.attrib.vertices[i];
			}

			int numTriangles = 0;
			for (int i = 0; i < myTOL.shapes.size(); i++) {

				const tinyobj::shape_t &shape = myTOL.shapes[i];

				numTriangles += shape.mesh.num_face_vertices.size();
			}

			unsigned* indexbuffer = (unsigned*) rtcSetNewGeometryBuffer(
					embree.geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), numTriangles);

			int c = 0;
			for (int m = 0; m < myTOL.shapes.size(); m++) {
				const tinyobj::shape_t &shape = myTOL.shapes[m];

				assert(shape.mesh.indices.size() == 3 * shape.mesh.num_face_vertices.size());

				for(int i = 0; i < shape.mesh.indices.size(); i++) {

					assert(shape.mesh.indices[i].vertex_index >= 0);

					indexbuffer[c++] = shape.mesh.indices[i].vertex_index;
				}
			}

			embree.commit();
		}
		~obj() {}

		/*
		 * Function - hit
		 *
		 * Parameters:
		 * [p] const Ray &ray,
		 * [p] float distance_min,
		 * [p] float distance_max,
		 * [p] hit_record &record
		 * 
		 * Return type:
		 * bool
		 * 
		 * Use:
		 * Hit is Hittable based funtion. 
		 * Hit_record update. 
		 * If ray hits obj (triangle) hit returns 1 (true), 
		 * else 0 (false).
		 */

		bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {

			RTCIntersectContext context;
			RTCRayHit rayhit;

			rayhit.ray.org_x = ray.get_origin().getX();
			rayhit.ray.org_y = ray.get_origin().getY();
			rayhit.ray.org_z = ray.get_origin().getZ();

			rayhit.ray.dir_x = ray.get_direction().getX();
			rayhit.ray.dir_y = ray.get_direction().getY();
			rayhit.ray.dir_z = ray.get_direction().getZ();

			rayhit.ray.tnear = distance_min;
			rayhit.ray.tfar  = distance_max;

			rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

			rtcInitIntersectContext(&context);

			rtcIntersect1(embree.scene, &context, &rayhit);

			if(rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID) {
				return false;
			}

			Vec normal(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
			float distance = rayhit.ray.tfar;

			record.u = rayhit.hit.u;
			record.v = rayhit.hit.v;

			record.normal = normal;
			Vec outward_normal = record.normal;
			record.set_face_normal(ray, outward_normal);

			record.material_ptr = material_ptr;
			record.point = ray.at(distance);
			record.distance = distance;

			return true;
		}

	private:
		shared_ptr<Material> material_ptr;

		std::string filePath;
		std::string basePath;

		MyEmbree &embree;
		MyTOL myTOL;
};
