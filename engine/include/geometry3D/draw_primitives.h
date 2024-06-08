#pragma once

#include <geometry3D/structs.h>
using namespace Physics;

#include <color.h>
using namespace Core::Maths;

void drawVector(vec3 position, vec3 vector, Color color);

void drawAxis();

void drawPlane(Plane plane, float scale, Color color);

void drawQuad(Quad quad, Color color);
void drawQuadWireframe(Quad quad, Color color);

void drawBox(Box box, Color color);
void drawBoxWireframe(Box box, Color color);


void drawSphere(Sphere sphere, int divisions, Color color);
void drawSpherePortion(Sphere sphere, int lats, int longs, int startTheta, int endTheta, int startPhi, int endPhi, Color color);
void drawSphereWireframe(Sphere sphere, int divisions, Color color);
void drawSpherePortionWireframe(Sphere sphere, int lats, int longs, int startTheta, int endTheta, int startPhi, int endPhi, Color color);


void drawCylinder(Cylinder cylinder, int divisions, Color color, bool displayCaps);
void drawCylinderPortion(Cylinder cylinder, int divisions, int startTheta, int endTheta, Color color, bool displayCaps);
void drawCylinderWireframe(Cylinder cylinder, int divisions, Color color, bool displayCaps);
void drawCylinderPortionWireframe(Cylinder cylinder, int divisions, int startTheta, int endTheta, Color color, bool displayCaps);

void drawCone(Cone cone, int divisions, Color color);

void drawCapsule(Capsule capsule, int divisions, Color color);
void drawCapsuleWireframe(Capsule capsule, int divisions, Color color);