#pragma once

#include <vec3.h>
#include <quaternion.h>
using namespace Core::Maths;


namespace Physics
{
	class Quad
	{
	public:
		Quaternion quaternion;
		vec3 position;
		vec3 extensions;

		Quad() = default;
		Quad(Quaternion quaternion, vec3 position, vec3 extensions);
	};
}