#include <component.h>
using namespace Engine;

#include <imgui.h>

#include <gameobject.h>

Component::~Component()
{
	gameObject->RemoveComponent(this);
}

void Component::setActive(bool value)
{
	if (m_enabled != value)
		value ? onEnable() : onDisable();

	m_enabled = value;
}

void Component::drawEditorWindow()
{
	ImGui::Text("=================");
	if (ImGui::Button("Delete component"))
	{
		delete this;
	}
}