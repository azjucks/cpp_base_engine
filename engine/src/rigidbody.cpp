#include <rigidbody.h>
using namespace Physics;

#include <time_manager.h>
using namespace Core;

#include <imgui.h>

#include <physics.h>

#include <gameobject.h>


Rigidbody::Rigidbody(GameObject* go)
	: Component(go)
{
	collider = gameObject->GetComponent<Collider>();

	if (collider == nullptr) // If no colliders on GameObject, we create one
		collider = gameObject->AddComponent<SphereCollider>();

	PhysicsEngine::instance()->registerRigidbody(this);
}

Rigidbody::~Rigidbody()
{
	PhysicsEngine::instance()->unregisterRigidbody(this);
	Component::~Component();
}

vec3 Rigidbody::computeNextPosition()
{
	// acceleration = sum of all forces acting on the rigidbody

	vec3 gravity_acceleration = vec3{ 0.f, GRAVITY * gravityMultiplier, 0.f };

	acceleration = (forceExternal + gravity_acceleration) / mass;
	forceExternal = { 0, 0, 0 };

	// velocity update according to acceleration
	velocity = speedFromNewton(velocity, acceleration, TimeManager::instance()->fixedDeltaTime);

	// New potential position IF no collision
	return positionFromNewton(gameObject->transform->position, velocity, TimeManager::instance()->fixedDeltaTime);
}

void Rigidbody::updatePosition()
{
	gameObject->transform->position = positionFromNewton(gameObject->transform->position, velocity, TimeManager::instance()->fixedDeltaTime);
}

void Rigidbody::drawEditorWindow()
{
	if (ImGui::TreeNode("Component: Rigidbody"))
	{
		ImGui::DragFloat("Mass", &mass, 1.0f, 100.0f);
		ImGui::DragFloat("Drag", &drag, 0.0f, 1.0f);
		ImGui::DragFloat("Gravity Multiplier", &gravityMultiplier, 0.0f, 10.0f);

		ImGui::Text("Force:");
		std::string forx = "x: " + std::to_string(forceExternal.x);
		std::string fory = "y: " + std::to_string(forceExternal.y);
		std::string forz = "z: " + std::to_string(forceExternal.z);
		ImGui::Text(forx.c_str());
		ImGui::Text(fory.c_str());
		ImGui::Text(forz.c_str());
		ImGui::Text("Acceleration:");
		std::string accx = "x: " + std::to_string(acceleration.x);
		std::string accy = "y: " + std::to_string(acceleration.y);
		std::string accz = "z: " + std::to_string(acceleration.z);
		ImGui::Text(accx.c_str());
		ImGui::Text(accy.c_str());
		ImGui::Text(accz.c_str());
		ImGui::Text("Velocity:");
		std::string velx = "x: " + std::to_string(velocity.x);
		std::string vely = "y: " + std::to_string(velocity.y);
		std::string velz = "z: " + std::to_string(velocity.z);
		ImGui::Text(velx.c_str());
		ImGui::Text(vely.c_str());
		ImGui::Text(velz.c_str());


		Component::drawEditorWindow();
		ImGui::TreePop();
	}
}


std::string Rigidbody::getSaveFormat()
{
	return "component " + std::to_string((int)ComponentType::RIGIDBODY) + " "
		+ std::to_string(mass) + " "
		+ std::to_string(drag) + " "
		+ std::to_string(gravityMultiplier) + " "
		+ "\n";
}

void Rigidbody::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;

	iss >> mass;
	iss >> drag;
	iss >> gravityMultiplier;
}

void Rigidbody::addForce(const vec3& v)
{
	forceExternal += v * 100 * TimeManager::instance()->deltaTime;
}