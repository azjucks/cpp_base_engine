#include <geometry3D/box.h>
using namespace Physics;

Box::Box(Quaternion quaternion, vec3 position = { 0 }, vec3 extensions = { 0.5f }, float roundness = 0.0f)
{
	this->quaternion = quaternion;
	this->position = position;
	this->extensions = extensions;
	this->roundness = roundness;
}