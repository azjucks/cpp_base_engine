#include <geometry3D/collision.h>

#include <geometry3D/intersection.h>

bool collision_sphere_box(Sphere sphere, vec3 currentPos, vec3 nextPos, Box box, vec3& interPt, vec3& interNormal)
{
	vec3 ab = nextPos - currentPos;
	vec3 abLonger = normalized(ab) * (sphere.radius + box.roundness);
	currentPos = currentPos - abLonger;

	Box rBox = Box(box.quaternion, box.position, box.extensions + sphere.radius, box.roundness + sphere.radius);

	return (intersection_segment_box(currentPos, nextPos, rBox, interPt, interNormal));
}


bool is_value_in_range(float value, float min, float max)
{
	return value >= min && value <= max;
}

bool is_point_inside_box(vec3 point, Box box)
{
	return is_value_in_range(point.x, box.position.x - box.extensions.x, box.position.x + box.extensions.x) &&
		   is_value_in_range(point.y, box.position.y - box.extensions.y, box.position.y + box.extensions.y) &&
		   is_value_in_range(point.z, box.position.z - box.extensions.z, box.position.z + box.extensions.z);
}

bool is_box_inside_AABB(Box AABB, Box box)
{
	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				vec3 corner = { i * (box.extensions.x + box.roundness), j * (box.extensions.y + box.roundness), k * (box.extensions.z + box.roundness) };
				if (!is_point_inside_box(Vector3RotateByQuaternion(corner, box.quaternion) + box.position, AABB))
					return false;
			}
		}
	}

	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				vec3 corner = { i * (AABB.extensions.x + AABB.roundness), j * (AABB.extensions.y + AABB.roundness), k * (AABB.extensions.z + AABB.roundness) };
				if (is_point_inside_box(corner + AABB.position, box))
					return false;
			}
		}
	}

	return true;
}

bool is_sphere_intersecting_AABB(const Box& AABB, const Sphere& sphere)
{
	Box sphereBox = Box(QuaternionIdentity(), sphere.center, vec3{ sphere.radius, sphere.radius, sphere.radius }, 0.0f);	
	
	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				vec3 corner = { i * (sphereBox.extensions.x + sphereBox.roundness), j * (sphereBox.extensions.y + sphereBox.roundness), k * (sphereBox.extensions.z + sphereBox.roundness) };
				if (is_point_inside_box(Vector3RotateByQuaternion(corner, sphereBox.quaternion) + sphereBox.position, AABB))
					return true;
			}
		}
	}

	return false;
}