// #include <iostream>
// #include <fstream>
// #include <ctime>

// #include "GLFW_window_set.h"

// int main(int argc, char **argv) {

// 	window_setup();

// 	return 0;
// }

#include <embree3/rtcore.h>
#include <limits>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

tinyobj::attrib_t my_attrib;
std::vector<tinyobj::shape_t> my_shapes;
std::vector<tinyobj::material_t> my_materials;

static bool myLoadObj(const char* filename, const char* basepath = NULL,
                        bool triangulate = true) {
  std::cout << "Loading " << filename << std::endl;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

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
    printf("Failed to load/parse .obj.\n");
    return false;
  }

  my_attrib = attrib;
  my_shapes = shapes;
  my_materials = materials;

  return true;
  
}

int main()
{

  bool ok = myLoadObj("/home/vk/work/Ray-tracer/src/Models/Body_low.obj", "./Models/");

  int size = my_attrib.vertices.size() -1;



  RTCDevice device = rtcNewDevice(NULL);
  RTCScene scene   = rtcNewScene(device);
  RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);



  float* vb = (float*) rtcSetNewGeometryBuffer(geom,
      RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, size * sizeof(float), size / 3);
  for (int i = 0; i < my_attrib.vertices.size(); i++) {

    vb[i * 3] = my_attrib.vertices[i * 3];
    vb[i * 3 + 1] = my_attrib.vertices[i * 3 + 1];
    vb[i * 3 + 2] = my_attrib.vertices[i * 3 + 2];
  }

  unsigned* ib = (unsigned*) rtcSetNewGeometryBuffer(geom,
      RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, size / 3 * sizeof(unsigned), size / 3 / 3);
  for (int i = 0; i < size / 3; i++) {
    vb[i] = i;
  }



  rtcCommitGeometry(geom);
  rtcAttachGeometry(scene, geom);
  rtcReleaseGeometry(geom);
  rtcCommitScene(scene);



  RTCRayHit rayhit;
  rayhit.ray.org_x  = 0.25f; rayhit.ray.org_y = 0.25f; rayhit.ray.org_z = -1.f;
  rayhit.ray.dir_x  = 0.f; rayhit.ray.dir_y = 0.f; rayhit.ray.dir_z =  0.5f;
  rayhit.ray.tnear  = 0.f;
  rayhit.ray.tfar   = std::numeric_limits<float>::infinity();
  rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

  RTCIntersectContext context;
  rtcInitIntersectContext(&context);

  rtcIntersect1(scene, &context, &rayhit);

  if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    std::cout << "Intersection at t = " << rayhit.ray.tfar << std::endl;
  } else {
    std::cout << "No Intersection" << std::endl;
  }



  rtcReleaseScene(scene);
  rtcReleaseDevice(device);
}
// Can get the full code from https://github.com/V-KMilev/Ray-tracer.git