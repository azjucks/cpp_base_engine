#include "player.h"

#include <imgui.h>

#include <gameobject.h>
using namespace Engine;

#include <time_manager.h>
using namespace Core;

#include <physics.h>

void Player::awake()
{
	Input::instance()->actionsPressed.find(GLFW_KEY_W)->second = [&] { pInputs.moveForward = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_S)->second = [&] { pInputs.moveBackward = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_A)->second = [&] { pInputs.moveRight = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_D)->second = [&] { pInputs.moveLeft = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_SPACE)->second = [&] { if (rb != nullptr && isGrounded) rb->addForce({ 0, jumpForce, 0 }); };

	Input::instance()->actionsReleased.find(GLFW_KEY_W)->second = [&] { pInputs.moveForward = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_S)->second = [&] { pInputs.moveBackward = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_A)->second = [&] { pInputs.moveRight = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_D)->second = [&] { pInputs.moveLeft = false; };
}

void Player::start()
{
	rb = gameObject->GetComponent<Rigidbody>();
}

void Player::update()
{
	Collider* ground = PhysicsEngine::instance()->raycast(gameObject->transform->position, vec3{ 0, -1, 0 });
	isGrounded = ground != nullptr;

	float rotSpeed = 250.0f;

	if (pInputs.moveRight)
	{
		gameObject->transform->rotation.y += rotSpeed * TimeManager::instance()->deltaTime * DEG2RAD;
	}
	if (pInputs.moveLeft)
	{
		gameObject->transform->rotation.y -= rotSpeed * TimeManager::instance()->deltaTime * DEG2RAD;
	}
}

void Player::fixedUpdate()
{
	if (gameObject->transform->position.y < -50.0f)
	{
		gameObject->transform->position = { 0, 0, 0 };
		gameObject->transform->rotation = { 0, 0, 0 };
		rb->velocity = { 0, 0, 0 };
	}

	vec3 forward = Vector3RotateByQuaternion({ 0, 0, 1 }, QuaternionFromAxisAngle({ 0, 1, 0 }, gameObject->transform->rotation.y));
	forward = normalized(forward);


	vec3 input = { 0 };

	if (pInputs.moveForward)
		input += forward;
	if (pInputs.moveBackward)
		input -= forward;


	if (length(input) > 0.f)
	{
		vec3 inputDir = normalized(input) * moveSpeed;
		if (!isGrounded)
			inputDir *= 0.5f;

		rb->addForce(inputDir);
	}

	float velY = rb->velocity.y;
	rb->velocity.y = 0;

	float speed = length(rb->velocity);
	float maxSpeed = 10.0f;
	if (speed > maxSpeed)
	{
		rb->velocity = normalized(rb->velocity) * maxSpeed;
	}

	rb->velocity.y = velY;

}

void Player::drawEditorWindow()
{
	if (ImGui::TreeNode("Component: Player"))
	{
		ImGui::DragFloat("Move Speed", &moveSpeed, 1.0f, 100.0f);
		ImGui::DragFloat("Jump Force", &jumpForce, 1.0f, 100.0f);

		Component::drawEditorWindow();
		ImGui::TreePop();
	}
}

std::string Player::getSaveFormat()
{
	return "component " + std::to_string((int)ComponentType::PLAYER) + " "
		+ std::to_string(moveSpeed) + " "
		+ std::to_string(jumpForce) + " "
		+ "\n";
}

void Player::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;

	iss >> moveSpeed;
	iss >> jumpForce;
}