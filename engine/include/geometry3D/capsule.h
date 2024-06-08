#pragma once

#include <vec3.h>
using namespace Core::Maths;
#include <geometry3D/sphere.h>
#include <geometry3D/cylinder.h>

namespace Physics
{
	class Capsule
	{
	public:
		Quaternion quaternion;
		vec3 position;
		float height;
		float radius;

		Capsule() = default;
		Capsule(Quaternion quaternion, vec3 center, float height, float radius);

		void getColliders(Sphere& sphere1, Sphere& sphere2, Cylinder& cylinder);
	};

}