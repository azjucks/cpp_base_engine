#include <renderer.h>
using namespace LowRenderer;

#include <resources_manager.h>


// Lights
void Renderer::registerLight(Light* light)
{
	if (lights.size() < 8)
	{
		lights.push_back(light);
	}
}

void Renderer::unregisterLight(Light* light)
{
	for (int i = 0; i < lights.size(); ++i)
	{
		if (lights[i] == light)
		{
			lights[i] = lights.back();
			lights.pop_back();
			return;
		}
	}
}

void Renderer::setShaderLights()
{
	ResourcesManager* res = ResourcesManager::instance();

	for (std::unordered_map<std::string, Shader>::iterator it = res->shaders.begin(); it != res->shaders.end(); it++)
	{
		it->second.use();

		if (cameras.size() > 0)
		{
			it->second.setVec3("viewPos", cameras[0]->gameObject->transform->position);
		}
		else
		{
			it->second.setVec3("viewPos", { 0, 0, 0 });
		}

		for (int i = 0; i < lights.size(); ++i)
		{
			std::string n = std::to_string(i);
			it->second.setBool("lights[" + n + "].enabled", lights[i]->enabled);
			it->second.setBool("lights[" + n + "].directional", lights[i]->directional);
			it->second.setBool("lights[" + n + "].spotlight", lights[i]->spotlight);
			it->second.setVec3("lights[" + n + "].position", lights[i]->gameObject->transform->position);
			it->second.setVec3("lights[" + n + "].ambient", lights[i]->ambient);
			it->second.setVec3("lights[" + n + "].diffuse", lights[i]->diffuse);
			it->second.setVec3("lights[" + n + "].specular", lights[i]->specular);
			it->second.setVec3("lights[" + n + "].attenuation", lights[i]->attenuation);
		}
	}
}

// Cameras
void Renderer::registerCamera(Camera* cam)
{
	cameras.push_back(cam);
}

void Renderer::unregisterCamera(Camera* cam)
{
	for (int i = 0; i < cameras.size(); ++i)
	{
		if (cameras[i] == cam)
		{
			cameras[i] = cameras.back();
			cameras.pop_back();
			return;
		}
	}
}

void Renderer::updateViewProjs()
{
	for (Camera* cam : cameras)
	{
		mat4 proj = cam->getProjection();
		mat4 view = cam->getViewMatrix();
		cam->viewProj = proj * view;
	}
}

mat4 Renderer::getViewProj(int layer)
{
	for (Camera* cam : cameras)
	{
		if (cam->layer == layer)
		{
			return cam->viewProj;
		}
	}
	return Core::Maths::identity();
}