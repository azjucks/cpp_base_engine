#include <geometry3D/cone.h>
using namespace Physics;


Cone::Cone(vec3 ptA, vec3 ptB, float radius)
{
	this->ptA = ptA;
	this->ptB = ptB;
	this->radius = radius;
}