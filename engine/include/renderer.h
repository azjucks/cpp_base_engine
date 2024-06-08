#pragma once

#include <singleton.h>

#include <light.h>
#include <camera.h>

#include <vector>

namespace LowRenderer
{
	class Renderer : public Singleton<Renderer>
	{
		friend class Singleton<Renderer>;

	protected:
		~Renderer() override {
			lights.clear();
			cameras.clear();
		}
	public:
		std::vector<Light*> lights;
		std::vector<Camera*> cameras;

		// Lights
		void registerLight(Light* light);
		void unregisterLight(Light* light);

		void setShaderLights();

		// Cameras
		void registerCamera(Camera* cam);
		void unregisterCamera(Camera* cam);

		//Camera* getMainCamera();

		void updateViewProjs();

		mat4 getViewProj(int layer);

	};
}