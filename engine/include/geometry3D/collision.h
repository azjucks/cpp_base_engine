#pragma once

#include <geometry3D/structs.h>
using namespace Physics;

bool collision_sphere_box(Sphere sphere, vec3 currentPos, vec3 nextPos, Box box, vec3& interPt, vec3& interNormal);

bool is_box_inside_AABB(Box AABB, Box box);

bool is_sphere_intersecting_AABB(const Box& AABB, const Sphere& sphere);