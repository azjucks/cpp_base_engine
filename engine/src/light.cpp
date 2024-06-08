#include <light.h>

#include <imgui.h>

#include <renderer.h>
using namespace LowRenderer;

#include <gameobject.h>
using namespace Engine;

Light::Light(GameObject* go)
	: Component(go)
{
	Renderer::instance()->registerLight(this);
}

Light::~Light()
{
	Renderer::instance()->unregisterLight(this);
	Component::~Component();
}

void Light::drawEditorWindow()
{
	if (ImGui::TreeNode("Component: Light"))
	{
		ImGui::Checkbox("Enable", &enabled);
		if (enabled)
		{
			ImGui::Checkbox("Directional", &directional);
			ImGui::Checkbox("Spotlight", &spotlight);

			ImGui::DragFloat3("Light Position", gameObject->transform->position.e);
			ImGui::ColorEdit3("Light Ambient color", ambient.e);
			ImGui::ColorEdit3("Light Diffuse color", diffuse.e);
			ImGui::ColorEdit3("Light Specular color", specular.e);

			ImGui::Text("Light attenuation :");

			ImGui::SliderFloat("Constant ", &attenuation.x, 0.001f, 1.00f);
			ImGui::SliderFloat("Linear   ", &attenuation.y, 0.001f, 1.00f);
			ImGui::SliderFloat("Quadratic", &attenuation.z, 0.001f, 1.00f);
		}

		ImGui::TreePop();
	}


}

std::string Light::getSaveFormat()
{
	return "component " + std::to_string((int)ComponentType::LIGHT) + " "
	+ std::to_string(enabled) + " "
	+ std::to_string(directional) + " "
	+ std::to_string(spotlight) + " "
	+ std::to_string(ambient.x) + " "
	+ std::to_string(ambient.y) + " "
	+ std::to_string(ambient.z) + " "
	+ std::to_string(diffuse.x) + " "
	+ std::to_string(diffuse.y) + " "
	+ std::to_string(diffuse.z) + " "
	+ std::to_string(specular.x) + " "
	+ std::to_string(specular.y) + " "
	+ std::to_string(specular.z) + " "
	+ std::to_string(attenuation.x) + " "
	+ std::to_string(attenuation.y) + " "
	+ std::to_string(attenuation.z) + " "
	+ "\n";
}

void Light::loadFromSaveFormat(const std::string& line)
{
	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;

	iss >> enabled;
	iss >> directional;
	iss >> spotlight;
	iss >> ambient.x;
	iss >> ambient.y;
	iss >> ambient.z;
	iss >> diffuse.x;
	iss >> diffuse.y;
	iss >> diffuse.z;
	iss >> specular.x;
	iss >> specular.y;
	iss >> specular.z;
	iss >> attenuation.x;
	iss >> attenuation.y;
	iss >> attenuation.z;
}