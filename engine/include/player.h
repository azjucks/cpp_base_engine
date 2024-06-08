#pragma once

#include "component.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "maths.h"
#include "input.h"

namespace Engine
{
	class Player : public Component
	{
	protected:
		struct PlayerInputs
		{
			bool moveForward = false;
			bool moveBackward = false;
			bool moveRight = false;
			bool moveLeft = false;
		};

	private:
		Rigidbody* rb = nullptr;
		PlayerInputs pInputs = PlayerInputs();

		bool isGrounded = false;

	public:
		Player() = default;
		Player(GameObject* go)
			: Component(go) { }

		float moveSpeed = 50.0f;
		float jumpForce = 100.0f;

		void awake() override;
		void start() override;
		void update() override;
		void fixedUpdate() override;

		void drawEditorWindow() override;

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;
	};
}
