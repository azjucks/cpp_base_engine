#pragma once

#include "component.h"
using namespace Engine;

namespace Physics
{
	enum class ColliderType
	{
		BOX,
		SPHERE
	};

	struct PhysicsMaterial
	{
		float friction = 0.02f;
		float bounciness = 0.00f;
	};

	class Collider : public Component
	{
	public:
		ColliderType type = ColliderType::BOX;
		PhysicsMaterial physicsMat;

		bool matchScale = true;

		// Constructors
		Collider() = default;
		Collider(GameObject * go, ColliderType colliderType)
			: Component(go), type(colliderType) {}
		~Collider() override;

		void drawEditorWindow() override;
	};
}