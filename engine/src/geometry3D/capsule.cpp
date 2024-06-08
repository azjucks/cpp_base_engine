#include <geometry3D/capsule.h>
using namespace Physics;


Capsule::Capsule(Quaternion quaternion, vec3 position, float height, float radius)
{
	this->quaternion = quaternion;
	this->position = position;
	this->height = height;
	this->radius = radius;
}


void Capsule::getColliders(Sphere& sphere1, Sphere& sphere2, Cylinder& cylinder)
{
	vec3 axis = Vector3RotateByQuaternion({ 0, 1, 0 }, quaternion);

	vec3 diff = axis * height * 0.5f;
	vec3 ptA = position - diff;
	vec3 ptB = position + diff;

	sphere1.quaternion = quaternion;
	sphere2.quaternion = quaternion;
	sphere1.center = ptA;
	sphere2.center = ptB;
	sphere1.radius = radius;
	sphere2.radius = radius;
	cylinder = Cylinder(quaternion, ptA, ptB, radius);
}