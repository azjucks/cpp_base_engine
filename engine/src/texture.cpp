#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <texture.h>
using namespace Resources;

#include <iostream>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>


Texture::Texture(const std::string& filepath, bool flip)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set wrapping / filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Then load and generate the texture

	// Flip texture on y-axis if needed
	stbi_set_flip_vertically_on_load(flip);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	if (!data)
	{
		cout << "Failed to load texture." << endl;
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}