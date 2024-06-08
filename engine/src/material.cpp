#include <material.h>
using namespace Resources;

Material::Material()
{
	ambient = { 0.3f, 0.3f, 0.3f };
	diffuse = { 1.0f, 1.0f, 1.0f };
	specular = { 1.0f, 1.0f, 1.0f };
	emissive = { 0.0f, 0.0f, 0.0f };
	shininess = 5.0f;
}