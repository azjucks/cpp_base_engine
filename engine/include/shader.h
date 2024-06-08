#pragma once

#include <iostream>
using namespace std;

#include <maths.h>
using namespace Core::Maths;

namespace Resources
{
	class Shader
	{
	public:
		unsigned int ID = 0;

		std::string vertexPath;
		std::string fragmentPath;

		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		void checkCompileErrors(unsigned int shader, string type);

		void use();
		void setBool(const string& name, bool value) const;
		void setInt(const string& name, int value) const;
		void setFloat(const string& name, float value) const;
		void setVec2(const string& name, vec2 value) const;
		void setVec3(const string& name, vec3 value) const;
		void setVec4(const string& name, vec4 value) const;
	};
}