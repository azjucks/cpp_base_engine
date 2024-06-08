#pragma once

#include <vec3.h>
#include <quaternion.h>
using namespace Core::Maths;

namespace Physics
{
	class Cylinder
	{
	public:
		Quaternion quaternion = QuaternionIdentity();
		vec3 ptA = { 0, 0, 0 };
		vec3 ptB = { 0, 0, 0 };
		float radius = 0.5f;

		Cylinder() = default;
		Cylinder(vec3 ptA, vec3 ptB, float radius);
		Cylinder(Quaternion quaternion, vec3 ptA, vec3 ptB, float radius);
		Cylinder(Quaternion quaternion, vec3 base, float height, float radius);
	};
}