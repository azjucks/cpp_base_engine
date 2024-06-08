#pragma once

#include <mat4.h>
using namespace Core;
using namespace Core::Maths;

#include <string>

#include <component.h>
using namespace Engine;

namespace Physics
{
	class Transform : public Component
	{
	public:
		Transform* parent = nullptr;

		Transform() = default;
		Transform(GameObject* go);

		vec3 position = { 0, 0, 0 };
		vec3 rotation = { 0, 0, 0 };
		vec3 scale = { 1, 1, 1 };

		mat4 modelMatrix = Core::Maths::identity();

		void updateTransform();
		mat4 getModelUpdated();

		std::string getSaveFormat();
		void loadFromSaveFormat(const std::string& line);
	};
}
