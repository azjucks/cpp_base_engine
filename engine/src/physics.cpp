#include <physics.h>
using namespace Physics;

#include <geometry3D/collision.h>

#include <time_manager.h>
#include <algorithm>

using namespace Core;

#include <gameobject.h>
using namespace Engine;

void PhysicsEngine::registerCollider(Collider* collider)
{
	dynamicColliders.push_back(collider);
}

void PhysicsEngine::registerRigidbody(Rigidbody* rb)
{
	rigidbodies.push_back(rb);
}

void PhysicsEngine::unregisterCollider(Collider* collider)
{
	for (int i = 0; i < dynamicColliders.size(); ++i)
	{
		if (dynamicColliders[i] == collider)
		{
			dynamicColliders[i] = dynamicColliders.back();
			dynamicColliders.pop_back();
			return;
		}
	}
}

void PhysicsEngine::unregisterRigidbody(Rigidbody* rb)
{
	for (int i = 0; i < rigidbodies.size(); ++i)
	{
		if (rigidbodies[i] == rb)
		{
			rigidbodies[i] = rigidbodies.back();
			rigidbodies.pop_back();
			return;
		}
	}
}


void PhysicsEngine::updatePhysicsEngine()
{
	rigidbodies.erase(std::remove(rigidbodies.begin(), rigidbodies.end(), nullptr), rigidbodies.end());
	dynamicColliders.erase(std::remove(dynamicColliders.begin(), dynamicColliders.end(), nullptr), dynamicColliders.end());

	processCollisions();
}


void PhysicsEngine::processCollisions()
{
	for (Rigidbody* rb : rigidbodies)
	{
		vec3 ptA = rb->gameObject->transform->position;
		vec3 ptB = rb->computeNextPosition();

		SphereCollider* rbCollider = (SphereCollider*)rb->collider;

		if (rbCollider == nullptr)
			continue;

		int i = 0;
		while (i < dynamicColliders.size())
		{
			BoxCollider* col = (BoxCollider*)dynamicColliders[i];

			if (col == nullptr)
				continue;


			vec3 interPt;
			vec3 interNormal;

			Sphere sphere = rbCollider->collider;
			sphere.center = ptA + vec3{0, 0.5f, 0};

			Box box = col->collider;
			box.position = col->gameObject->transform->position;
			box.quaternion = QuaternionFromEuler(col->gameObject->transform->rotation);
			box.extensions = col->gameObject->transform->scale * 0.5f;

			if (collision_sphere_box(sphere, ptA, ptB, box, interPt, interNormal))
			{
				//vec3 distanceToCollider = interPt - ptA;
				//float lengthToCollider = length(distanceToCollider);
				//if (lengthToCollider > 0.001f)
				//{
				//	float velocityLength = length(rb->velocity);
				//	float rapport = lengthToCollider / velocityLength;
				//	rb->velocity *= rapport;
				//	break;
				//}

				// Create referential around velocity/normal plan
				vec3 axisY = normalized(interNormal);
				vec3 axisZ = normalized(cross_product(axisY, rb->velocity));
				vec3 axisX = normalized(cross_product(axisY, axisZ));

				// Calculate velocity projected on the referential
				float pY = dot_product(rb->velocity, axisY);
				float pX = dot_product(rb->velocity, axisX);

				// Compute collision reaction
				vec3 reactionForce = axisY * fabsf(pY);
				vec3 tangeantForce = axisX * fabsf(pX) * col->physicsMat.friction;

				rb->velocity = (rb->velocity + reactionForce + tangeantForce);

				// Compute bounce angle
				//vec3 reflectForce = rb->velocity - interNormal * 2 * velocityProjOnNormal;
				// Bounce & friction forces according to physics material
				//vec3 bounceVelocity = reflectForce * (col->physicsMat.bounciness);
				// 
				//rb->gameObject->transform->position = interPt;

				ptB = rb->gameObject->transform->position + rb->velocity * TimeManager::instance()->fixedDeltaTime;
			}
			++i;

		}

		rb->updatePosition();
	}
}

#include <geometry3D/intersection.h>

Collider* PhysicsEngine::raycast(const vec3& origin, const vec3& ray)
{
	int i = 0;
	while (i < dynamicColliders.size())
	{
		BoxCollider* col = (BoxCollider*)dynamicColliders[i];

		if (col == nullptr)
			continue;

		vec3 interPt;
		vec3 interNormal;


		Box box = col->collider;
		box.position = col->gameObject->transform->position;
		box.quaternion = QuaternionFromEuler(col->gameObject->transform->rotation);
		box.extensions = col->gameObject->transform->scale * 0.5f;

		if (intersection_segment_box(origin, origin + ray, box, interPt, interNormal))
		{
			return col;
		}

		++i;
	}

	return nullptr;
}

