#pragma once

#include <vec3.h>
using namespace Core::Maths;
#include <quaternion.h>

namespace Physics
{
	class Sphere
	{
	public:
		Quaternion quaternion = QuaternionIdentity();
		vec3 center = { 0, 0, 0 };
		float radius = 0.5f;

		Sphere() = default;
		Sphere(vec3 center, float radius);
	};
}