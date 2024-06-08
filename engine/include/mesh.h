#pragma once

#include <iostream>
using namespace std;

#include <maths.h>
using namespace Core::Maths;

#include <vector>

namespace Resources
{
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};

	class Mesh
	{
	public:
		unsigned int VAO = 0;
		vector<Vertex> vertices;

		void setUpBuffer();
	};

}