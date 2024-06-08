#pragma once

#include <string>

namespace Resources
{
	class Texture
	{
	public:
		unsigned int ID = 0;

		Texture() = default;
		Texture(const std::string& filepath, bool flip);
	};
	
}