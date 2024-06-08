#pragma once

#include <vec3.h>
using namespace Core::Maths;

namespace Physics
{
	class Cone
	{
	public:
		vec3 ptA;
		vec3 ptB;
		float radius;

		Cone(vec3 ptA, vec3 ptB, float radius);
	};
}