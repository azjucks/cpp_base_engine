#pragma once

#include <maths.h>
using namespace Core::Maths;

#include <transform.h>
using namespace Physics;

#include <component.h>
using namespace Engine;

namespace LowRenderer
{
	class Light : public Component
	{
	public:
		vec3 ambient = { 1, 1, 1 };
		vec3 diffuse = { 1, 1, 1 };
		vec3 specular = { 1, 1, 1 };
		vec3 attenuation = { 0.001f, 0.001f, 0.001f };

		bool directional = false;
		bool spotlight = false;
		bool enabled = true;


		Light() = default;
		Light(GameObject* go);
		~Light() override;

		// IMGUI INTERFACE
		void drawEditorWindow() override;

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;
	};
}

