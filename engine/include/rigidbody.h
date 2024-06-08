#pragma once

#include <component.h>

#include <collider.h>
#include <sphere_collider.h>
#include <box_collider.h>

namespace Physics
{
#define GRAVITY -10.0f

	class Rigidbody : public Component
	{
	public:
		Collider* collider = nullptr;

		float mass = 1.0f;
		float drag = 0.2f;
		float gravityMultiplier = 1.0f;

		vec3 forceExternal = { 0, 0, 0 };
		vec3 acceleration = { 0, 0, 0 };
		vec3 velocity = { 0, 0, 0 };


		Rigidbody() = default;
		Rigidbody(GameObject* go);
		~Rigidbody() override;

		vec3 computeNextPosition();
		void updatePosition();

		void drawEditorWindow() override;

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;

		void addForce(const vec3& v);

	private:
		inline vec3 speedFromNewton(vec3 currentSpeed, vec3 acceleration, float deltaTime)
		{
			return currentSpeed + acceleration * deltaTime;
		}

		inline vec3 positionFromNewton(vec3 currentPos, vec3 currentSpeed, float deltaTime)
		{
			return currentPos + currentSpeed * deltaTime;
		}

	};
}