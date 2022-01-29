#pragma once

#include <embree3/rtcore.h>

class MyEmbree {
	public:
		MyEmbree() {}
		~MyEmbree() {}

		void setup(RTCGeometryType type) {
			device = rtcNewDevice(NULL);
			scene = rtcNewScene(device);
			geom = rtcNewGeometry(device, type);
		}

		void commit() {
			rtcCommitGeometry(geom);
			rtcAttachGeometry(scene, geom);
			rtcReleaseGeometry(geom);
			rtcCommitScene(scene);
		}

		void release() {
			rtcReleaseScene(scene);
			rtcReleaseDevice(device);
		}

	public:
		RTCDevice device;
		RTCScene scene;
		RTCGeometry geom;
};
