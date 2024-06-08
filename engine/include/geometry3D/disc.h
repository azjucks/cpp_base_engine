#pragma once

#include <vec3.h>
using namespace Core::Maths;

namespace Physics
{
	struct Disc
	{
		vec3 normal;
		vec3 center;
		float radius;
	};
}