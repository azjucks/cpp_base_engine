#pragma once

#include <vector>
#include <vec3.h>

#include <geometry3D/structs.h>
using namespace Physics;

class TreeNode
{
public:
	Box AABB;

	std::vector<TreeNode> children;

	std::vector<Box*> colliders;

	TreeNode() = default;
	TreeNode(vec3 center, float halfSize);

	void generate_octree(float depth);

	void scan_colliders(std::vector<Box*>& boxes);

	//void draw(Color color);

	void get_sphere_potential_colliders(const Sphere& sphere, std::vector<Box*>& boxes);
};