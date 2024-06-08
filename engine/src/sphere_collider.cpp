#include <sphere_collider.h>
using namespace Physics;

#include <imgui.h>

#include <gameobject.h>

void SphereCollider::drawEditorWindow()
{
	if (ImGui::TreeNode("Sphere Collider"))
	{
		ImGui::Checkbox("Match scale", &matchScale);

		if (!matchScale)
		{
			float rad = collider.radius;
			ImGui::DragFloat("Radius", &rad, 0, 100.0f);
			collider.radius = rad;
		}

		Component::drawEditorWindow();
		ImGui::TreePop();
	}
}


std::string SphereCollider::getSaveFormat()
{
	return "component " + std::to_string((int)ComponentType::COLLIDER_SPHERE) + " "
		+ std::to_string(collider.radius)
		+ "\n";
}

void SphereCollider::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;

	iss >> collider.radius;
}