#include <geometry3D/plane.h>
using namespace Physics;



Plane::Plane(vec3 normal, float distance)
{
	this->normal = normalized(normal);
	this->distance = distance;
}

Plane::Plane(vec3 normal, vec3 pt)
{
	float D = dot_product(pt, normal);
	float distance = D / length(normal);

	*this = Plane(normal, distance);
}

Plane::Plane(vec3 s1, vec3 s2, vec3 s3)
{
	vec3 u = s2 - s1;
	vec3 v = s3 - s1;

	vec3 normal = cross_product(u, v);

	*this = Plane(normal, s1);
}

/*
void Plane::draw(vec3 position, float scale, Color color)
{
	rlPushMatrix();

	rlTranslatef(position.x, position.y, position.z);

	vec3 unitAxis = normal;

	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(QuaternionFromVector3ToVector3({ 0, 1, 0 }, unitAxis.toRlVec()), &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	rlScalef(scale, scale, scale);

	Vector3 a = { -0.5f, 0.0f, -0.5f };
	Vector3 b = { -0.5f, 0.0f,  0.5f };
	Vector3 c = {  0.5f, 0.0f,  0.5f };
	Vector3 d = {  0.5f, 0.0f, -0.5f };

	DrawSphere(a, 0.01f, RED);
	DrawSphere(b, 0.01f, GREEN);
	DrawSphere(c, 0.01f, BLUE);
	DrawSphere(d, 0.01f, YELLOW);

	DrawTriangle3D(a, b, c, color);
	DrawTriangle3D(a, c, b, color);
	DrawTriangle3D(c, d, a, color);
	DrawTriangle3D(c, a, d, color);

	rlPopMatrix();
}
*/