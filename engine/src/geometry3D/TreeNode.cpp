#include <geometry3D/TreeNode.h>

#include <geometry3D/collision.h>

TreeNode::TreeNode(vec3 center, float halfSize)
{
	AABB = Box(QuaternionIdentity(), center, { halfSize, halfSize, halfSize }, 0.0f);
}

void TreeNode::generate_octree(float depth)
{
	if (depth == 0)
		return;

	// Create children
	float childrenHalfSize = AABB.extensions.x * 0.5f;

	for (int i = -1; i < 2; i += 2)
	{
		for (int j = -1; j < 2; j += 2)
		{
			for (int k = -1; k < 2; k += 2)
			{
				// Add child
				vec3 position = vec3{ (float)i, (float)j, (float)k } * childrenHalfSize;
				children.push_back(TreeNode(position, childrenHalfSize));

				// Give it the boxes that he owns
				// and get back the leftovers (by reference)
				children.back().scan_colliders(colliders);

				// If it has no colliders registered, we simply delete it
				if (children.back().colliders.empty())
					children.pop_back();
				else // we iterate the algorithm on it
					children.back().generate_octree(depth - 1);
			}
		}
	}

}


void TreeNode::scan_colliders(std::vector<Box*>& boxes)
{
	int i = 0;
	while (i < boxes.size())
	{
		if (is_box_inside_AABB(AABB, *boxes[i]))
		{
			colliders.push_back(boxes[i]);

			boxes[i] = boxes.back();
			boxes.pop_back();
			this->scan_colliders(boxes);
			return;
		}
		else
		{
			i++;
		}
	}
}
/*
#include <draw_primitives.h>

void TreeNode::draw(Color color)
{
	drawBoxWireframe(AABB, color);
	for (TreeNode& child : children)
	{
		child.draw(color);
	}
}
*/

void TreeNode::get_sphere_potential_colliders(const Sphere& sphere, std::vector<Box*>& boxes)
{
	if (!is_sphere_intersecting_AABB(AABB, sphere))
		return;

	for (int i = 0; i < colliders.size(); ++i)
	{
		boxes.push_back(colliders[i]);
	}

	for (int i = 0; i < children.size(); ++i)
	{
		children[i].get_sphere_potential_colliders(sphere, boxes);
	}
}