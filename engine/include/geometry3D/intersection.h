#pragma once

#include <vec3.h>
using namespace Core::Maths;
#include <geometry3D/structs.h>
using namespace Physics;

bool intersection_segment_plane(vec3 ptA, vec3 ptB, Plane plane, vec3& interPt, vec3& interNormal);

bool intersection_segment_quad(vec3 ptA, vec3 ptB, Quad quad, vec3& interPt, vec3& interNormal);

bool intersection_segment_box(vec3 ptA, vec3 ptB, Box box, vec3& interPt, vec3& interNormal);

bool intersection_segment_sphere(vec3 ptA, vec3 ptB, Sphere sphere, vec3& interPt, vec3& interNormal);

bool intersection_segment_cylinder_infinite(vec3 ptA, vec3 ptB, Cylinder cylinder, vec3& interPt, vec3& interNormal);

bool intersection_segment_disc(vec3 ptA, vec3 ptB, Disc disc, vec3& interPt, vec3& interNormal);

bool intersection_segment_cylinder(vec3 ptA, vec3 ptB, Cylinder cylinder, vec3& interPt, vec3& interNormal);

bool intersection_segment_capsule(vec3 ptA, vec3 ptB, Capsule caps, vec3& interPt, vec3& interNormal);