#include <geometry3D/quad.h>
using namespace Physics;

#include <quaternion.h>

Quad::Quad(Quaternion quaternion, vec3 position, vec3 extensions)
{
	this->quaternion = quaternion;
	this->position = position;
	this->extensions = extensions;
}