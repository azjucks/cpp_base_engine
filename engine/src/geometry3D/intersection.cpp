#include <geometry3D/intersection.h>


#include <maths.h>
#include <quaternion.h>
using namespace Core::Maths;


static float epsilon = 0.000001f;


bool intersection_segment_plane(vec3 ptA, vec3 ptB, Plane plane, vec3& interPt, vec3& interNormal)
{
	vec3 ab = ptB - ptA;

	float proj1 = dot_product(ab, plane.normal);

	if (fabs(proj1) < epsilon)
		return false;

	vec3 p = plane.normal * plane.distance;
	vec3 ap = p - ptA;

	float proj2 = dot_product(ap, plane.normal);

	float t = proj2 / proj1;

	if (t < 0 || t > 1)
		return false;

	interPt = ptA + ab * t;
	interNormal = plane.normal;

	return true;
}

bool intersection_segment_quad(vec3 ptA, vec3 ptB, Quad quad, vec3& interPt, vec3& interNormal)
{
	vec3 normal = Vector3RotateByQuaternion({ 0, 1, 0 }, quad.quaternion);

	if (dot_product(normal, ptB - ptA) > 0)
		return false;

	Plane plane(normal, quad.position);

	if (!intersection_segment_plane(ptA, ptB, plane, interPt, interNormal))
		return false;
	vec3 vect = interPt - quad.position;

	vec3 i = normalized(Vector3RotateByQuaternion({ 1, 0, 0 }, quad.quaternion));
	vec3 j = normalized(Vector3RotateByQuaternion({ 0, 0, 1 }, quad.quaternion));

	float proj1 = dot_product(vect, i);
	float proj2 = dot_product(vect, j);

	if (fabs(proj1) > quad.extensions.x)
		return false;

	if (fabs(proj2) > quad.extensions.z)
		return false;

	return true;
}


bool intersection_segment_sphere(vec3 ptA, vec3 ptB, Sphere sphere, vec3& interPt, vec3& interNormal)
{
	vec3 ab = ptB - ptA;

	vec3 ca = ptA - sphere.center;

	// At² + Bt + C = 0;
	float A = dot_product(ab, ab);
	float B = dot_product(ca, ab) * 2;
	float C = dot_product(ca, ca) - sphere.radius * sphere.radius;

	float delta = B * B - 4 * A * C;

	if (delta < 0) // No solution
		return false;

	// 2 solutions but we consider only the first intersect
	float sdelta = sqrtf(delta);

	float t = (-B - sdelta) / (2 * A);

	if (t < 0 || t > 1)
		return false;

	interPt = ptA + ab * t;

	interNormal = (interPt - sphere.center) / sphere.radius;

	return true;
}

bool intersection_segment_cylinder_infinite(vec3 ptA, vec3 ptB, Cylinder cylinder, vec3& interPt, vec3& interNormal)
{
	vec3 ab = ptB - ptA;
	vec3 pa = ptA - cylinder.ptA;
	vec3 pq = cylinder.ptB - cylinder.ptA;

	float dot_ab = dot_product(ab, ab);
	float dot_pq = dot_product(pq, pq);
	float dot_pa = dot_product(pa, pa);
	float dot_pq_ab = dot_product(pq, ab);
	float dot_pa_pq = dot_product(pa, pq);
	float dot_pa_ab = dot_product(pa, ab);

	float A = dot_pq * dot_ab - dot_pq_ab * dot_pq_ab;

	if (A < epsilon && A > -epsilon)
		return false;

	float C = dot_pq * (dot_pa - cylinder.radius * cylinder.radius) - dot_pa_pq * dot_pa_pq;

	if (C < 0)
		return false;

	float B = dot_pq * dot_pa_ab - dot_pq_ab * dot_pa_pq;

	float delta = B * B - A * C;

	if (delta <= 0)
		return false;

	float t = (-B - sqrtf(delta)) / A;

	if (t < 0 || t > 1)
		return false;

	interPt = ptA + ab * t;
	interNormal = {};

	vec3 tiPt;
	vec3 tiNm;
	Plane plan(normalized(pq), interPt);

	if (intersection_segment_plane(cylinder.ptA, cylinder.ptB, plan, tiPt, tiNm))
	{
		interNormal = interPt - tiPt;
	}

	return true;
}

bool intersection_segment_cylinder(vec3 ptA, vec3 ptB, Cylinder cylinder, vec3& interPt, vec3& interNormal)
{
	vec3 ab = ptB - ptA;
	vec3 pa = ptA - cylinder.ptA;
	vec3 pq = cylinder.ptB - cylinder.ptA;

	float dot_ab = dot_product(ab, ab);
	float dot_pq = dot_product(pq, pq);
	float dot_pa = dot_product(pa, pa);
	float dot_pq_ab = dot_product(pq, ab);
	float dot_pa_pq = dot_product(pa, pq);
	float dot_pa_ab = dot_product(pa, ab);

	float A = dot_pq * dot_ab - dot_pq_ab * dot_pq_ab;
	float C = dot_pq * (dot_pa - cylinder.radius * cylinder.radius) - dot_pa_pq * dot_pa_pq;

	if (fabs(A) < epsilon && C > 0)
		return false;

	if (C < 0)
	{
		if (dot_pa_pq > 0 && dot_pa_pq < dot_pq)
			return false;

		if (dot_pa_pq < 0)
			return intersection_segment_disc(ptA, ptB, { -pq, cylinder.ptA, cylinder.radius }, interPt, interNormal);
		if (dot_pa_pq > dot_pq)
			return intersection_segment_disc(ptA, ptB, { pq, cylinder.ptB, cylinder.radius }, interPt, interNormal);
	}

	float B = dot_pq * dot_pa_ab - dot_pq_ab * dot_pa_pq;

	float delta = B * B - A * C;

	if (delta < 0)
		return false;

	float t = (-B - sqrtf(delta)) / A;

	if (t < 0 || t > 1)
		return false;

	interPt = ptA + ab * t;
	interNormal = {};

	vec3 pi = interPt - cylinder.ptA;
	float dot_pi_pq = dot_product(pi, pq);

	if (dot_pi_pq < 0)
		return intersection_segment_disc(interPt, ptB, { -pq, cylinder.ptA, cylinder.radius }, interPt, interNormal);
	if (dot_pi_pq > dot_pq)
		return intersection_segment_disc(interPt, ptB, { pq, cylinder.ptB, cylinder.radius }, interPt, interNormal);

	vec3 tiPt;
	vec3 tiNm;
	Plane plan(normalized(pq), interPt);

	if (intersection_segment_plane(cylinder.ptA, cylinder.ptB, plan, tiPt, tiNm))
	{
		interNormal = interPt - tiPt;
	}

	return true;
}

bool intersection_segment_disc(vec3 ptA, vec3 ptB, Disc disc, vec3& interPt, vec3& interNormal)
{
	Plane p = Plane(disc.normal, disc.center);
	if (!intersection_segment_plane(ptA, ptB, p, interPt, interNormal))
		return false;

	vec3 oi = interPt - disc.center;
	if (dot_product(oi, oi) > disc.radius * disc.radius)
		return false;

	return true;
}

bool intersection_segment_capsule(vec3 ptA, vec3 ptB, Capsule caps, vec3& interPt, vec3& interNormal)
{
	Sphere s1, s2;
	Cylinder cyl;
	caps.getColliders(s1, s2, cyl);

	vec3 ab = ptB - ptA;
	vec3 pa = ptA - cyl.ptA;
	vec3 pq = cyl.ptB - cyl.ptA;

	float dot_ab = dot_product(ab, ab);
	float dot_pq = dot_product(pq, pq);
	float dot_pa = dot_product(pa, pa);
	float dot_pq_ab = dot_product(pq, ab);
	float dot_pa_pq = dot_product(pa, pq);
	float dot_pa_ab = dot_product(pa, ab);

	float A = dot_pq * dot_ab - dot_pq_ab * dot_pq_ab;
	float C = dot_pq * (dot_pa - cyl.radius * cyl.radius) - dot_pa_pq * dot_pa_pq;

	if (fabs(A) < epsilon && C > 0)
		return false;

	if (C < 0)
	{
		if (dot_pa_pq > 0 && dot_pa_pq < dot_pq)
			return false;

		if (dot_pa_pq < 0)
			return intersection_segment_sphere(ptA, ptB, s1, interPt, interNormal);
		if (dot_pa_pq > dot_pq)
			return intersection_segment_sphere(ptA, ptB, s2, interPt, interNormal);
	}

	float B = dot_pq * dot_pa_ab - dot_pq_ab * dot_pa_pq;

	float delta = B * B - A * C;

	if (delta < 0)
		return false;

	float t = (-B - sqrtf(delta)) / A;

	if (t < 0 || t > 1)
		return false;

	interPt = ptA + ab * t;
	interNormal = {};

	vec3 pi = interPt - cyl.ptA;
	float dot_pi_pq = dot_product(pi, pq);

	if (dot_pi_pq < 0)
		return intersection_segment_sphere(interPt, ptB, s1, interPt, interNormal);
	if (dot_pi_pq > dot_pq)
		return intersection_segment_sphere(interPt, ptB, s2, interPt, interNormal);

	vec3 tiPt;
	vec3 tiNm;
	Plane plan(normalized(pq), interPt);

	if (intersection_segment_plane(cyl.ptA, cyl.ptB, plan, tiPt, tiNm))
	{
		interNormal = normalized(interPt - tiPt);
	}

	return true;
}

bool intersection_segment_box_face(vec3 ptA, vec3 ptB, Box box, Quad quad, vec3& interPt, vec3& interNormal)
{
	// No roundness case
	if (box.roundness <= 0.0f)
		return true;

	// We re-do intersection quad with voronoi face because we need the projection values

	// We redefine our quad as a voronoi face
	quad.extensions = { quad.extensions.x - box.roundness, quad.extensions.y - box.roundness, quad.extensions.z - box.roundness };
	vec3 normal = Vector3RotateByQuaternion({ 0, 1, 0 }, quad.quaternion);

	// We check if it is pointing in the right direction
	if (dot_product(normal, ptB - ptA) > 0)
		return false;

	// We check if it is intersecting with the plane of the quad
	Plane plane(normal, quad.position);
	if (!intersection_segment_plane(ptA, ptB, plane, interPt, interNormal))
		return false;


	// We first find the quad referential (i, normal, k) with normal being the up (j) vector
	vec3 i = normalized(Vector3RotateByQuaternion({ 1, 0, 0 }, quad.quaternion));
	vec3 k = normalized(Vector3RotateByQuaternion({ 0, 0, 1 }, quad.quaternion));

	// We project the intersection point on the quad referential
	vec3 vect = interPt - quad.position;
	float projX = dot_product(vect, i);
	float projZ = dot_product(vect, k);

	bool isInsideOnX = fabs(projX) < quad.extensions.x;
	bool isInsideOnZ = fabs(projZ) < quad.extensions.z;

	// If we are in the voronoi face -> intersection !
	if (isInsideOnX && isInsideOnZ)
		return true;

	// Then we are not in the voronoi face,
	// So we should be in one of the 4 capsules around the quad

	// We analyze de sign of the projection to get the position of the supposely capsules
	int signX = projX > 0 ? 1 : -1;
	int signZ = projZ > 0 ? 1 : -1;

	// If the interPoint is good in X-axis,
	// we should have only 1 capsule to check
	if (isInsideOnX)
	{
		Quaternion rotation = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f));
		vec3 position = quad.position - (normal * box.roundness) + k * quad.extensions.z * (float)signZ;
		Capsule caps = Capsule(rotation, position, quad.extensions.x * 2, box.roundness);

		return (intersection_segment_capsule(ptA, ptB, caps, interPt, interNormal));
	}

	// If the interPoint is good in Z-axis,
	// we should have only 1 capsule to check
	if (isInsideOnZ)
	{
		//Quaternion mult = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle(normal.toRlVec(), PI * 0.5f));
		Quaternion rotation = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f));
		vec3 position = quad.position - (normal * box.roundness) + i * quad.extensions.x * (float)signX;
		Capsule caps = Capsule(rotation, position, quad.extensions.z * 2, box.roundness);

		return (intersection_segment_capsule(ptA, ptB, caps, interPt, interNormal));
	}

	// If it is not, we double check with the 2 capsules
	// To get the intersection supposedly being in the capsule sphere
	Quaternion rotationZ = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f));
	vec3 positionZ = quad.position - (normal * box.roundness) + i * quad.extensions.x * (float)signX;
	Capsule capsZ = Capsule(rotationZ, positionZ, quad.extensions.z * 2, box.roundness);

	Quaternion rotationX = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f));
	vec3 positionX = quad.position - (normal * box.roundness) + k * quad.extensions.z * (float)signZ;
	Capsule capsX = Capsule(rotationX, positionX, quad.extensions.x * 2, box.roundness);

	// If at least one intersect, we got it !
	if (intersection_segment_capsule(ptA, ptB, capsX, interPt, interNormal)
		|| intersection_segment_capsule(ptA, ptB, capsZ, interPt, interNormal))
		return true;

	return false;
}

bool intersection_segment_box(vec3 ptA, vec3 ptB, Box box, vec3& interPt, vec3& interNormal)
{
	float extensionMin = getMin(box.extensions);
	box.roundness = clamp(box.roundness, 0, extensionMin);

	vec3 i = normalized(Vector3RotateByQuaternion({ 1,0,0 }, box.quaternion));
	vec3 j = normalized(Vector3RotateByQuaternion({ 0,1,0 }, box.quaternion));
	vec3 k = normalized(Vector3RotateByQuaternion({ 0,0,1 }, box.quaternion));
	vec3 AB = ptB - ptA;

	Quad quad;

	if (dot_product(k, AB) < 0.f)
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, PI * 0.5f)),
			Vector3RotateByQuaternion({ 0,0,box.extensions.z }, box.quaternion) + box.position,
			vec3{ box.extensions.x, 0.f, box.extensions.y });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}
	else
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, -PI * 0.5f)),
			Vector3RotateByQuaternion({ 0,0,-box.extensions.z }, box.quaternion) + box.position,
			vec3{ box.extensions.x, 0.f, box.extensions.y });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}

	if (dot_product(i, AB) < 0.f)
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 0, 0, 1 }, -PI * 0.5f)),
			Vector3RotateByQuaternion({ box.extensions.x, 0, 0 }, box.quaternion) + box.position,
			vec3{ box.extensions.y, 0.f, box.extensions.z });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}
	else
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 0, 0, 1 }, PI * 0.5f)),
			Vector3RotateByQuaternion({ -box.extensions.x, 0, 0 }, box.quaternion) + box.position,
			vec3{ box.extensions.y, 0.f, box.extensions.z });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}

	if (dot_product(j, AB) < 0.f)
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, 0.f)),
			Vector3RotateByQuaternion({ 0, box.extensions.y, 0 }, box.quaternion) + box.position,
			vec3{ box.extensions.x, 0.f, box.extensions.z });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}
	else
	{
		quad = Quad(QuaternionMultiply(box.quaternion, QuaternionFromAxisAngle({ 1, 0, 0 }, PI)),
			Vector3RotateByQuaternion({ 0,-box.extensions.y, 0 }, box.quaternion) + box.position,
			vec3{ box.extensions.x, 0.f, box.extensions.z });

		if (intersection_segment_quad(ptA, ptB, quad, interPt, interNormal))
		{
			//drawQuad(quad, YELLOW);
			return intersection_segment_box_face(ptA, ptB, box, quad, interPt, interNormal);
		}
	}

	return false;
}
