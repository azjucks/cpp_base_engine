#pragma once

#include <collider.h>
#include <geometry3D/box.h>
#include <geometry3D/draw_primitives.h>

namespace Physics
{
	class BoxCollider : public Collider
	{
	public:
		Box collider = Box();

		// Constructors
		BoxCollider() = default;
		BoxCollider(GameObject* go);
		~BoxCollider() override;

		void drawEditorWindow() override;

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;
	};
}