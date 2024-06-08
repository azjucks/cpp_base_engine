#pragma once

#include <vec3.h>
using namespace Core::Maths;
#include <quaternion.h>

namespace Physics
{
	class Box
	{
	public:
		Quaternion quaternion = QuaternionIdentity();
		vec3 position = { 0, 0, 0 };
		vec3 extensions = { 0.5f, 0.5f, 0.5f };
		float roundness = 0.0f; // WE NEED roundness < min(extensions) / 2;

		Box() = default;
		Box(Quaternion quaternion, vec3 position, vec3 extensions, float roundness);
	};
}

