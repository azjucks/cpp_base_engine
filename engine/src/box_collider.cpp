#include <box_collider.h>
using namespace Physics;
#include <physics.h>

#include <imgui.h>

#include <gameobject.h>


BoxCollider::BoxCollider(GameObject* go)
	: Collider(go, ColliderType::BOX)
{
	PhysicsEngine::instance()->registerCollider(this);
}

BoxCollider::~BoxCollider()
{
	PhysicsEngine::instance()->unregisterCollider(this);
	Component::~Component();
}


void BoxCollider::drawEditorWindow()
{
	if (ImGui::TreeNode("Box Collider"))
	{
		ImGui::Checkbox("Match scale", &matchScale);

		if (!matchScale)
		{
			vec3 ext = collider.extensions;
			ImGui::DragFloat3("Extensions", ext.e, 0, 100.0f);
			collider.extensions = ext;
		}

		Collider::drawEditorWindow();
		ImGui::TreePop();
	}
}

std::string BoxCollider::getSaveFormat()
{
	return "component " + std::to_string((int)ComponentType::COLLIDER_BOX) + " " 
		+ "\n";
}

void BoxCollider::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;
}