#include <geometry3D/cylinder.h>
using namespace Physics;

#include <quaternion.h>

#include <vector>


Cylinder::Cylinder(vec3 ptA, vec3 ptB, float radius)
{
	this->quaternion = QuaternionFromVector3ToVector3({ 0, 1, 0 }, normalized(ptB - ptA));
	this->ptA = ptA;
	this->ptB = ptB;
	this->radius = radius;
}

Cylinder::Cylinder(Quaternion quaternion, vec3 ptA, vec3 ptB, float radius)
{
	this->quaternion = quaternion;
	this->ptA = ptA;
	this->ptB = ptB;
	this->radius = radius;
}

Cylinder::Cylinder(Quaternion quaternion, vec3 base, float height, float radius)
{
	this->quaternion = quaternion;
	this->ptA = base;
	this->ptB = base + Vector3RotateByQuaternion({ 0, 1, 0 }, quaternion) * height;
	this->radius = radius;
}