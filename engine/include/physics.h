#pragma once

#include <singleton.h>

#include <rigidbody.h>

#include <vector>




namespace Physics
{
	class PhysicsEngine : public Singleton<PhysicsEngine>
	{
		friend class Singleton<PhysicsEngine>;

	public:
		//std::vector<Collider*> staticColliders; // Static colliders are set in an Octree for performance
		std::vector<Collider*> dynamicColliders; // Dynamic colliders are always checked because they are unpredictable

		std::vector<Rigidbody*> rigidbodies; // Rigidbodies are dynamic colliders that are using physics to move

		void registerCollider(Collider* collider);
		void registerRigidbody(Rigidbody* rb);

		void unregisterCollider(Collider* collider);
		void unregisterRigidbody(Rigidbody* rb);


		void updatePhysicsEngine();

		void processCollisions();

		Collider* raycast(const vec3& origin, const vec3& ray);

		// We need to loop on every rigidbodies:
		// For each rigidbody :
		//		1. Compute its next position
		//		2. Check intersection with all dynamic & other rigidbodies
		//		//Check collision with all static (with octree)

	private:
		



	};
}