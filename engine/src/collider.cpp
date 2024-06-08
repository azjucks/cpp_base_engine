#include <collider.h>
using namespace Physics;

#include <physics.h>

#include <imgui.h>

#include <gameobject.h>
using namespace Engine;

Collider::~Collider()
{
	PhysicsEngine::instance()->unregisterCollider(this);
	Component::~Component();
}

void Collider::drawEditorWindow()
{
	ImGui::Text("Physics Material:");
	ImGui::DragFloat("Friction", &physicsMat.friction, 0, 1.0f);
	ImGui::DragFloat("Bounciness", &physicsMat.bounciness, 0, 1.0f);

	Component::drawEditorWindow();
}