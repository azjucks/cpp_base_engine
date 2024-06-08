#pragma once

#include <maths.h>
using namespace Core::Maths;

namespace Resources
{
	class Material
	{
	public:
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		vec3 emissive;
		float shininess;

		Material();
	};
}
