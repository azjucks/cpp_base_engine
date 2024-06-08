#pragma once

#include <vec3.h>
using namespace Core::Maths;

namespace Physics
{
	class Plane
	{
	public:
		vec3 normal = {};
		float distance = 0.0f;

		Plane() = default;
		Plane(vec3 normal, float distance);
		Plane(vec3 normal, vec3 pt);
		Plane(vec3 s1, vec3 s2, vec3 s3);
	};
}