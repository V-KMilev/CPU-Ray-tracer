#pragma once

#include <embree3/rtcore.h>

class MyEmbree {
	public:
		MyEmbree() {}
		~MyEmbree() {}

		/*
		 * Function - setup
		 *
		 * Parameters:
		 * RTCGeometryType type (enum var)
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling setup you set up
		 * RTCDevice, RTCScene and RTCGeometry.
		 * Embree setup.
		 */

		void setup(enum RTCGeometryType type) {
			device = rtcNewDevice(NULL);
			scene = rtcNewScene(device);
			geom = rtcNewGeometry(device, type);
		}

		/*
		 * Function - commit
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling commit you commit 
		 * the geometry, attach the scene with 
		 * the committed geometry, release the 
		 * committed geometry and commit the 
		 * scene.
		 * Embree setup.
		 */

		void commit() {
			rtcCommitGeometry(geom);
			rtcAttachGeometry(scene, geom);
			rtcReleaseGeometry(geom);
			rtcCommitScene(scene);
		}

		/*
		 * Function - release
		 *
		 * Parameters:
		 * [p] RTCGeometryType type (enum var)
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling release you release 
		 * the committed scene and the device.
		 * Embree setup.
		 */

		void release() {
			rtcReleaseScene(scene);
			rtcReleaseDevice(device);
		}

	public:
		RTCDevice device;
		RTCScene scene;
		RTCGeometry geom;
};
