#pragma once

#include <collider.h>
#include <geometry3D/sphere.h>
#include <geometry3D/draw_primitives.h>

namespace Physics
{
	class SphereCollider : public Collider
	{
	public:
		Sphere collider = Sphere();

		// Constructors
		SphereCollider() = default;
		SphereCollider(GameObject* go)
			: Collider(go, ColliderType::SPHERE) {}

		void drawEditorWindow() override;

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;
	};
}