
#include <geometry3D/draw_primitives.h>

#include <maths.h>
#include <color.h>
#include <vector>

#include <GLFW/glfw3.h>

using namespace Core::Maths;

// =================
// BASIC SHAPES DRAW

void drawVector(vec3 position, vec3 vector, Color color)
{
	glColor4ub(color.x, color.y, color.z, color.w);

	glBegin(GL_LINES);

	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x + vector.x, position.y + vector.y, position.z + vector.z);

	glEnd();
}

void drawTriangle(const vec3& a, const vec3& b, const vec3& c, Color color)
{
	glColor4ub(color.x, color.y, color.z, color.w);

	glBegin(GL_TRIANGLES);

	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);

	glEnd();
}

void drawAxis()
{
	vec3 x = { 1, 0, 0 };
	vec3 y = { 0, 1, 0 };
	vec3 z = { 0, 0, 1 };

	Cylinder axisX = Cylinder(QuaternionFromVector3ToVector3(y, x), -x * 500, x * 500, 0.1f);
	Cylinder axisY = Cylinder(QuaternionFromVector3ToVector3(y, y), -y * 500, y * 500, 0.1f);
	Cylinder axisZ = Cylinder(QuaternionFromVector3ToVector3(y, z), -z * 500, z * 500, 0.1f);

	drawCylinder(axisX, 7, RED, false);
	drawCylinder(axisY, 7, GREEN, false);
	drawCylinder(axisZ, 7, BLUE, false);

	//drawCone(Cone(x * 25, x * 28, 0.5f), 11, RED);
	//drawCone(Cone(y * 25, y * 28, 0.5f), 11, GREEN);
	//drawCone(Cone(z * 25, z * 28, 0.5f), 11, BLUE);
}

// PLANE
// -----
void drawPlane(Plane plane, float scale, Color color)
{
	glPushMatrix();

	vec3 position = plane.normal * plane.distance;

	vec3 unitAxis = plane.normal;

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(QuaternionFromVector3ToVector3({ 0, 1, 0 }, unitAxis), &vect, &angle);

	glTranslatef(position.x, position.y, position.z);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	glScalef(scale, 1.0f, scale);

	vec3 a = { -0.5f, 0.0f, -0.5f };
	vec3 b = { -0.5f, 0.0f,  0.5f };
	vec3 c = { 0.5f, 0.0f,  0.5f };
	vec3 d = { 0.5f, 0.0f, -0.5f };


	drawTriangle(a, b, c, color);
	drawTriangle(a, c, b, color);
	drawTriangle(c, d, a, color);
	drawTriangle(c, a, d, color);

	glPopMatrix();
}

// QUAD
// ----
void drawQuad(vec3 position, vec3 scale, Quaternion q, Color color)
{
	Quad quad(q, position, scale);
	drawQuad(quad, color);
}

void drawQuadWireframe(vec3 position, vec3 scale, Quaternion q, Color color)
{
	Quad quad(q, position, scale);
	drawQuadWireframe(quad, color);
}

void drawQuad(Quad quad, Color color)
{
	//int vertexCount = 6;

	glPushMatrix();

	glTranslatef(quad.position.x, quad.position.y, quad.position.z);

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(quad.quaternion, &vect, &angle);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	glScalef(quad.extensions.x, 0.0f, quad.extensions.z);

	vec3 a = { -1.0f, 0.0f, -1.0f };
	vec3 b = { -1.0f, 0.0f,  1.0f };
	vec3 c = { 1.0f, 0.0f,  1.0f };
	vec3 d = { 1.0f, 0.0f, -1.0f };

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_TRIANGLES);

	drawTriangle(a, b, c, color);
	drawTriangle(c, d, a, color);

	glEnd();

	glPopMatrix();
}

void drawQuadWireframe(Quad quad, Color color)
{
	//int vertexCount = 8;

	glPushMatrix();

	glTranslatef(quad.position.x, quad.position.y, quad.position.z);

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(quad.quaternion, &vect, &angle);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	glScalef(quad.extensions.x, 0.0f, quad.extensions.z);

	vec3 a = { -1.0f, 0.0f, -1.0f };
	vec3 b = { -1.0f, 0.0f,  1.0f };
	vec3 c = { 1.0f, 0.0f,  1.0f };
	vec3 d = { 1.0f, 0.0f, -1.0f };

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_LINES);

	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);

	glVertex3f(a.x, a.y, a.z);
	glVertex3f(d.x, d.y, d.z);

	glVertex3f(d.x, d.y, d.z);
	glVertex3f(c.x, c.y, c.z);

	glEnd();

	glPopMatrix();
}

// BOX
// ---
void drawSimpleBox(Box box, Color color)
{
	Quaternion mult;
	Quaternion xAxis90 = QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f);
	Quaternion xAxis90neg = QuaternionFromAxisAngle({ 1, 0, 0 }, -PI * 0.5f);
	Quaternion zAxis90 = QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f);
	Quaternion zAxis90neg = QuaternionFromAxisAngle({ 0, 0, 1 }, -PI * 0.5f);
	Quaternion xAxis = QuaternionFromAxisAngle({ 1, 0, 0 }, 0.f);
	Quaternion xAxis180 = QuaternionFromAxisAngle({ 1, 0, 0 }, PI);

	mult = QuaternionMultiply(box.quaternion, xAxis90);
	drawQuad(Vector3RotateByQuaternion({ 0, 0, box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, xAxis90neg);
	drawQuad(Vector3RotateByQuaternion({ 0, 0, -box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, zAxis90);
	drawQuad(Vector3RotateByQuaternion({ -box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, zAxis90neg);
	drawQuad(Vector3RotateByQuaternion({ box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, xAxis);
	drawQuad(Vector3RotateByQuaternion({ 0, box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, xAxis180);
	drawQuad(Vector3RotateByQuaternion({ 0, -box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

}

void drawSimpleBoxWireframe(Box box, Color color)
{
	Quaternion mult;
	Quaternion front = QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f);
	Quaternion back = QuaternionFromAxisAngle({ 1, 0, 0 }, -PI * 0.5f);
	Quaternion left = QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f);
	Quaternion right = QuaternionFromAxisAngle({ 0, 0, 1 }, -PI * 0.5f);
	Quaternion up = QuaternionFromAxisAngle({ 1, 0, 0 }, 0.f);
	Quaternion down = QuaternionFromAxisAngle({ 1, 0, 0 }, PI);

	mult = QuaternionMultiply(box.quaternion, front);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, 0, box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, back);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, 0, -box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, left);
	drawQuadWireframe(Vector3RotateByQuaternion({ -box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, right);
	drawQuadWireframe(Vector3RotateByQuaternion({ box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, up);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, down);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, -box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);
}


void drawBox(Box box, Color color)
{
	float extensionMin = getMin(box.extensions);
	box.roundness = clamp(box.roundness, 0, extensionMin);
	if (box.roundness <= 0.001f)
		drawSimpleBox(box, color);

	Quaternion mult;
	Quaternion xAxis90 = QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f);
	Quaternion xAxis90neg = QuaternionFromAxisAngle({ 1, 0, 0 }, -PI * 0.5f);
	Quaternion zAxis90 = QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f);
	Quaternion zAxis90neg = QuaternionFromAxisAngle({ 0, 0, 1 }, -PI * 0.5f);
	Quaternion xAxis = QuaternionFromAxisAngle({ 1, 0, 0 }, 0.f);
	Quaternion xAxis180 = QuaternionFromAxisAngle({ 1, 0, 0 }, PI);

	// We pre-calculate box extensions - roundness;
	vec3 voronoiExtensions = { (box.extensions.x - box.roundness), (box.extensions.y - box.roundness), (box.extensions.z - box.roundness) };

	Cylinder c;
	Sphere s;
	s.quaternion = box.quaternion;
	s.radius = box.roundness;


	mult = QuaternionMultiply(box.quaternion, xAxis90);
	drawQuad(Vector3RotateByQuaternion({ 0, 0, box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.z * 2, box.roundness);
	drawCylinderPortion(c, 20, 0, 5, Color{ 255, color.y, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ -voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.z * 2, box.roundness);
	drawCylinderPortion(c, 20, 5, 10, Color{ 255, color.y, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ -voronoiExtensions.x, voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.z * 2, box.roundness);
	drawCylinderPortion(c, 20, 10, 15, Color{ 255, color.y, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.z * 2, box.roundness);
	drawCylinderPortion(c, 20, 15, 20, Color{ 255, color.y, color.z, 255 }, false);

	mult = QuaternionMultiply(box.quaternion, xAxis90neg);
	drawQuad(Vector3RotateByQuaternion({ 0, 0, -box.extensions.z }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, zAxis90);
	drawQuad(Vector3RotateByQuaternion({ -box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.x * 2, box.roundness);
	drawCylinderPortion(c, 20, 0, 5, Color{ color.x, color.y, 255, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.x * 2, box.roundness);
	drawCylinderPortion(c, 20, 5, 10, Color{ color.x, color.y, 255, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.x * 2, box.roundness);
	drawCylinderPortion(c, 20, 10, 15, Color{ color.x, color.y, 255, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.x * 2, box.roundness);
	drawCylinderPortion(c, 20, 15, 20, Color{ color.x, color.y, 255, 255 }, false);

	mult = QuaternionMultiply(box.quaternion, zAxis90neg);
	drawQuad(Vector3RotateByQuaternion({ box.extensions.x, 0, 0 }, box.quaternion) + box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, xAxis);
	drawQuad(Vector3RotateByQuaternion({ 0, box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.y * 2, box.roundness);
	drawCylinderPortion(c, 20, 0, 5, Color{ color.x, 255, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ -voronoiExtensions.x, -voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.y * 2, box.roundness);
	drawCylinderPortion(c, 20, 5, 10, Color{ color.x, 255, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ -voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.y * 2, box.roundness);
	drawCylinderPortion(c, 20, 10, 15, Color{ color.x, 255, color.z, 255 }, false);
	c = Cylinder(mult, Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position, voronoiExtensions.y * 2, box.roundness);
	drawCylinderPortion(c, 20, 15, 20, Color{ color.x, 255, color.z, 255 }, false);

	mult = QuaternionMultiply(box.quaternion, xAxis180);
	drawQuad(Vector3RotateByQuaternion({ 0, -box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	Color sCol = Color{ (color.x + 25), (color.y + 25), (color.z + 25), 255 };
	// Draw 8 spheres chunks
	s.center = Vector3RotateByQuaternion({ voronoiExtensions.x, voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 0, 4, 0, 4, sCol);
	s.center = Vector3RotateByQuaternion({ -voronoiExtensions.x, voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 0, 4, 5, 9, sCol);
	s.center = Vector3RotateByQuaternion({ -voronoiExtensions.x, -voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 5, 10, 5, 9, sCol);
	s.center = Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 5, 10, 0, 4, sCol);
	s.center = Vector3RotateByQuaternion({ voronoiExtensions.x, voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 0, 4, 15, 19, sCol);
	s.center = Vector3RotateByQuaternion({ -voronoiExtensions.x, voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 0, 4, 10, 14, sCol);
	s.center = Vector3RotateByQuaternion({ -voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 5, 10, 10, 14, sCol);
	s.center = Vector3RotateByQuaternion({ voronoiExtensions.x, -voronoiExtensions.y, -voronoiExtensions.z }, box.quaternion) + box.position;
	drawSpherePortion(s, 10, 20, 5, 10, 15, 19, sCol);
}

void drawBoxWireframe(Box box, Color color)
{
	float extensionMin = getMin(box.extensions);
	box.roundness = clamp(box.roundness, 0, extensionMin);
	if (box.roundness == 0)
		drawSimpleBoxWireframe(box, color);

	Quaternion mult;
	Quaternion front = QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f);
	Quaternion back = QuaternionFromAxisAngle({ 1, 0, 0 }, -PI * 0.5f);
	Quaternion left = QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f);
	Quaternion right = QuaternionFromAxisAngle({ 0, 0, 1 }, -PI * 0.5f);
	Quaternion up = QuaternionFromAxisAngle({ 1, 0, 0 }, 0.f);
	Quaternion down = QuaternionFromAxisAngle({ 1, 0, 0 }, PI);

	vec3 corner = { (box.extensions.x - box.roundness), (box.extensions.y - box.roundness), (box.extensions.z - box.roundness) };

	Sphere s;
	s.quaternion = box.quaternion;
	s.radius = box.roundness;

	Cylinder c;
	c.quaternion = box.quaternion;
	c.radius = box.roundness;

	mult = QuaternionMultiply(box.quaternion, front);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, 0, box.extensions.z }, box.quaternion)+ box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);


	mult = QuaternionMultiply(box.quaternion, back);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, 0, -box.extensions.z }, box.quaternion)+ box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.y - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, left);
	drawQuadWireframe(Vector3RotateByQuaternion({ -box.extensions.x, 0, 0 }, box.quaternion)+ box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);


	mult = QuaternionMultiply(box.quaternion, right);
	drawQuadWireframe(Vector3RotateByQuaternion({ box.extensions.x, 0, 0 }, box.quaternion)+box.position, { box.extensions.y - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	mult = QuaternionMultiply(box.quaternion, up);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, box.extensions.y, 0 }, box.quaternion)+ box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);


	mult = QuaternionMultiply(box.quaternion, down);
	drawQuadWireframe(Vector3RotateByQuaternion({ 0, -box.extensions.y, 0 }, box.quaternion) + box.position, { box.extensions.x - box.roundness, 0.f, box.extensions.z - box.roundness }, mult, color);

	// Draw 8 spheres chunks
	s.center = Vector3RotateByQuaternion({ corner.x, corner.y, corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 0, 9, 0, 9, color);
	s.center = Vector3RotateByQuaternion({ -corner.x, corner.y, corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 0, 9, 10, 19, color);
	s.center = Vector3RotateByQuaternion({ -corner.x, -corner.y, corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 10, 20, 10, 19, color);
	s.center = Vector3RotateByQuaternion({ corner.x, -corner.y, corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 10, 20, 0, 9, color);
	s.center = Vector3RotateByQuaternion({ corner.x, corner.y, -corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 0, 9, 30, 39, color);
	s.center = Vector3RotateByQuaternion({ -corner.x, corner.y, -corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 0, 9, 20, 29, color);
	s.center = Vector3RotateByQuaternion({ -corner.x, -corner.y, -corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 10, 20, 20, 29, color);
	s.center = Vector3RotateByQuaternion({ corner.x, -corner.y, -corner.z }, box.quaternion)+ box.position;
	drawSpherePortionWireframe(s, 20, 40, 10, 20, 30, 39, color);
}

// SPHERE
// ------
vec3 getPolarCoordinates(double r, double phi, double theta)
{
	return {
		(float)(r * sin(theta) * cos(phi)),
		(float)(r * cos(theta)),
		(float)(r * sin(theta) * sin(phi))
	};
}

void drawSphere(Sphere sphere, int divisions, Color color)
{
	drawSpherePortion(sphere, divisions, divisions, 0, divisions, 0, divisions, color);
}

void drawSphereWireframe(Sphere sphere, int divisions, Color color)
{
	drawSpherePortionWireframe(sphere, divisions, divisions, 0, divisions, 0, divisions, color);
}

void drawSpherePortion(Sphere sphere, int lats, int longs, int startTheta, int endTheta, int startPhi, int endPhi, Color color)
{
	//int vertexCount = (endTheta - startTheta + 1) * (endPhi - startPhi + 1) * 6;

	glPushMatrix();

	glTranslatef(sphere.center.x, sphere.center.y, sphere.center.z);

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.quaternion, &vect, &angle);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_TRIANGLES);

	float r = sphere.radius;

	for (int j = startTheta; j <= endTheta; ++j)
	{
		float v0 = ((float)j + 0) / (float)lats;
		float v1 = ((float)j + 1) / (float)lats;

		float theta0 = v0 * PI;
		float theta1 = v1 * PI;

		for (int i = startPhi; i <= endPhi; ++i)
		{
			float u0 = ((float)i + 0) / (float)longs;
			float u1 = ((float)i + 1) / (float)longs;

			float phi0 = u0 * 2 * PI;
			float phi1 = u1 * 2 * PI;

			vec3 f1 = getPolarCoordinates(r, phi0, theta0);
			vec3 f2 = getPolarCoordinates(r, phi0, theta1);
			vec3 f3 = getPolarCoordinates(r, phi1, theta1);
			vec3 f4 = getPolarCoordinates(r, phi1, theta0);

			glVertex3f(f1.x, f1.y, f1.z);
			glVertex3f(f3.x, f3.y, f3.z);
			glVertex3f(f2.x, f2.y, f2.z);

			glVertex3f(f3.x, f3.y, f3.z);
			glVertex3f(f1.x, f1.y, f1.z);
			glVertex3f(f4.x, f4.y, f4.z);
		}
	}

	glEnd();

	glPopMatrix();
}

void drawSpherePortionWireframe(Sphere sphere, int lats, int longs, int startTheta, int endTheta, int startPhi, int endPhi, Color color)
{
	//int vertexCount = (endTheta - startTheta + 1) * (endPhi - startPhi + 1) * 8;

	glPushMatrix();

	glTranslatef(sphere.center.x, sphere.center.y, sphere.center.z);

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.quaternion, &vect, &angle);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_LINES);

	float r = sphere.radius;

	for (int j = startTheta; j <= endTheta; ++j)
	{
		float v0 = ((float)j + 0) / (float)lats;
		float v1 = ((float)j + 1) / (float)lats;

		float theta0 = v0 * PI;
		float theta1 = v1 * PI;

		for (int i = startPhi; i <= endPhi; ++i)
		{
			float u0 = ((float)i + 0) / (float)longs;
			float u1 = ((float)i + 1) / (float)longs;

			float phi0 = u0 * 2 * PI;
			float phi1 = u1 * 2 * PI;

			vec3 f1 = (getPolarCoordinates(r, phi0, theta0));
			vec3 f2 = (getPolarCoordinates(r, phi0, theta1));
			vec3 f3 = (getPolarCoordinates(r, phi1, theta1));
			vec3 f4 = (getPolarCoordinates(r, phi1, theta0));

			glVertex3f(f1.x, f1.y, f1.z);
			glVertex3f(f2.x, f2.y, f2.z);

			glVertex3f(f3.x, f3.y, f3.z);
			glVertex3f(f2.x, f2.y, f2.z);

			glVertex3f(f1.x, f1.y, f1.z);
			glVertex3f(f4.x, f4.y, f4.z);

			glVertex3f(f3.x, f3.y, f3.z);
			glVertex3f(f4.x, f4.y, f4.z);
		}
	}

	glEnd();

	glPopMatrix();
}


// CYLINDER
// --------
void drawCylinder(Cylinder cylinder, int divisions, Color color, bool displayCaps)
{
	drawCylinderPortion(cylinder, divisions, 0, divisions, color, displayCaps);
}

void drawCylinderWireframe(Cylinder cylinder, int divisions, Color color, bool displayCaps)
{
	drawCylinderPortionWireframe(cylinder, divisions, 0, divisions, color, displayCaps);
}

void drawCylinderPortion(Cylinder cylinder, int divisions, int startTheta, int endTheta, Color color, bool displayCaps)
{
	//int vertexCount = (endTheta - startTheta + 1) * (6 + displayCaps ? 6 : 0);

	glPushMatrix();

	vec3 ab = cylinder.ptB - cylinder.ptA;

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.quaternion, &vect, &angle);

	glTranslatef(cylinder.ptA.x, cylinder.ptA.y, cylinder.ptA.z);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	glScalef(cylinder.radius, length(ab), cylinder.radius);

	// Create vertices buffer
	std::vector<vec3> vertices;

	float add = 2 * PI / (float)divisions;
	for (int i = startTheta; i <= endTheta; i++)
	{
		float x = cosf(add * i);
		float z = sinf(add * i);

		vec3 vA = { x, 0.0f, z };
		vec3 vB = { x, 1.0f, z };

		vertices.push_back(vA);
		vertices.push_back(vB);
	}

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_TRIANGLES);

	// Draw cylinder
	for (size_t i = 3; i < vertices.size(); i += 2)
	{
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		glVertex3f(vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z);
		glVertex3f(vertices[i - 2].x, vertices[i - 2].y, vertices[i - 2].z);

		glVertex3f(vertices[i - 2].x, vertices[i - 2].y, vertices[i - 2].z);
		glVertex3f(vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z);
		glVertex3f(vertices[i - 3].x, vertices[i - 3].y, vertices[i - 3].z);
	}

	// If wanted, draw the circles on each side
	if (displayCaps)
	{
		for (size_t idx = 3; idx <= vertices.size(); idx += 2)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(vertices[idx - 3].x, vertices[idx - 3].y, vertices[idx - 3].z);
			glVertex3f(vertices[idx - 1].x, vertices[idx - 1].y, vertices[idx - 1].z);

			glVertex3f(0, 1, 0);
			glVertex3f(vertices[idx].x, vertices[idx].y, vertices[idx].z);
			glVertex3f(vertices[idx - 2].x, vertices[idx - 2].y, vertices[idx - 2].z);
		}
	}

	glEnd();

	glPopMatrix();
}

void drawCylinderPortionWireframe(Cylinder cylinder, int divisions, int startTheta, int endTheta, Color color, bool displayCaps)
{
	//int vertexCount = (endTheta - startTheta + 1) * (12 + displayCaps ? 12 : 0);

	glPushMatrix();

	vec3 ab = cylinder.ptB - cylinder.ptA;

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.quaternion, &vect, &angle);


	glTranslatef(cylinder.ptA.x, cylinder.ptA.y, cylinder.ptA.z);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	glScalef(cylinder.radius, length(ab), cylinder.radius);


	// Create vertices buffer
	std::vector<vec3> vertices;

	float add = 2 * PI / (float)divisions;
	for (int i = startTheta; i <= endTheta; i++)
	{
		float x = cosf(add * i);
		float z = sinf(add * i);

		vec3 vA = { x, 0.0f, z };
		vec3 vB = { x, 1.0f, z };

		vertices.push_back(vA);
		vertices.push_back(vB);
	}

	glColor4ub(color.x, color.y, color.z, color.w);
	glBegin(GL_LINES);

	// Draw cylinder
	for (unsigned int k = 2; k < vertices.size(); ++k)
	{
		glVertex3f(vertices[k - 2].x, vertices[k - 2].y, vertices[k - 2].z);
		glVertex3f(vertices[k - 1].x, vertices[k - 1].y, vertices[k - 1].z);

		glVertex3f(vertices[k - 1].x, vertices[k - 1].y, vertices[k - 1].z);
		glVertex3f(vertices[k].x, vertices[k].y, vertices[k].z);

		glVertex3f(vertices[k].x, vertices[k].y, vertices[k].z);
		glVertex3f(vertices[k - 2].x, vertices[k - 2].y, vertices[k - 2].z);
	}

	glEnd();

	glPopMatrix();
}

void drawCapsule(Capsule capsule, int divisions, Color color)
{
	Sphere s1, s2;
	Cylinder cyl;
	capsule.getColliders(s1, s2, cyl);

	drawCylinder(cyl, divisions, color, false);
	drawSphere(s1, divisions, color);
	drawSphere(s2, divisions, color);
}


void drawCapsuleWireframe(Capsule capsule, int divisions, Color color)
{
	Sphere s1, s2;
	Cylinder cyl;
	capsule.getColliders(s1, s2, cyl);

	drawCylinderWireframe(cyl, divisions, color, false);
	drawSphereWireframe(s1, divisions, color);
	drawSphereWireframe(s2, divisions, color);
}


// TODO : Rework cone draw
void getReferential(const vec3 axe, vec3& i, vec3& j, vec3& k)
{
	j = normalized(axe);

	vec3 not_j = { 1, 0, 0 };
	if (j == not_j)
		not_j = { 0, 1, 0 };

	i = normalized(cross_product(j, not_j));
	k = cross_product(i, j);
}

void drawCone(Cone cone, int divisions, Color color)
{
	glPushMatrix();

	vec3 ab = cone.ptB - cone.ptA;
	vec3 unitAxis = normalized(ab);

	vec3 vect;
	float angle;
	QuaternionToAxisAngle(QuaternionFromVector3ToVector3({ 0, 1, 0 }, unitAxis), &vect, &angle);

	glTranslatef(cone.ptA.x, cone.ptA.y, cone.ptA.z);
	glRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	glScalef(cone.radius * 2, length(ab), cone.radius * 2);

	// Create vertices buffer
	std::vector<vec3> vertices;

	// Loop through circle equation to create cone
	float add = 2 * PI / divisions;
	for (float angle = 0; angle <= 2 * PI; angle += add)
	{
		float x = cosf(angle) * 0.5f;
		float z = sinf(angle) * 0.5f;

		vec3 pos = { x, 0.0f, z };

		vertices.push_back(pos);
	}

	// Loop through the vertices to draw base + cone
	for (size_t i = 1; i < vertices.size(); ++i)
	{
		//DrawTriangle3D({ 0, 0.0f, 0 }, vertices[i - 1], vertices[i], color);
		//DrawTriangle3D({ 0, 1.0f, 0 }, vertices[i], vertices[i - 1], color);
		drawTriangle({ 0, 0.0f, 0 }, vertices[i - 1], vertices[i], color);
		drawTriangle({ 0, 1.0f, 0 }, vertices[i], vertices[i - 1], color);

	}

	glPopMatrix();
}
