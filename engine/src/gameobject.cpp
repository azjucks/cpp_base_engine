#include <gameobject.h>
//using namespace Engine;

#include <component.h>

#include <physics.h>
#include <box_collider.h>
#include <sphere_collider.h>
#include <rigidbody.h>
#include <player.h>
using namespace Physics;

GameObject::~GameObject()
{
	for (Component* compo : m_components)
	{
		if (compo != nullptr)
			delete compo;
	}
}

void GameObject::awakeComponents()
{
	for (Component* compo : m_components)
	{
		if (!compo->hasAwaken)
		{
			compo->awake();
			compo->hasAwaken = true;
		}
	}
}

void GameObject::startComponents()
{
	for (Component* compo : m_components)
	{
		if (!compo->hasStarted)
		{
			compo->start();
			compo->hasStarted = true;
		}
	}
}

void GameObject::updateComponents()
{
	for (Component* compo : m_components)
	{
		compo->update();
	}
}

void GameObject::fixedUpdateComponents()
{
	for (Component* compo : m_components)
	{
		compo->fixedUpdate();
	}
}

void GameObject::lateUpdateComponents()
{
	for (Component* compo : m_components)
	{
		compo->lateUpdate();
	}
}

void GameObject::drawCallComponents()
{
	transform->updateTransform();

	for (Component* compo : m_components)
	{
		compo->drawCall();
	}
}

#include <imgui.h>

void GameObject::drawEditorWindow()
{
	static bool wantToRename;
	if (ImGui::Button("Rename"))
		wantToRename = true;

	if (wantToRename)
	{
		static char nameBuffer[128];
		ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
		name = nameBuffer;

		if (ImGui::Button("Ok"))
			wantToRename = false;
		
	}
	

	if (ImGui::TreeNode("Transform"))
	{
		vec3 rot = transform->rotation * RAD2DEG;

		ImGui::DragFloat3("Position", transform->position.e, 0.1f);
		ImGui::DragFloat3("Rotation", rot.e, 1.0f);
		ImGui::DragFloat3("Scale", transform->scale.e);

		transform->rotation = rot * DEG2RAD;


		if (ImGui::Button("Reset transform"))
		{
			transform->position = { 0, 0, 0 };
			transform->rotation = { 0, 0, 0 };
			transform->scale = { 1, 1, 1 };
		}

		// PARENT MODIF
		//
		/*
		static const char* currentGo = "";
		if (ImGui::BeginCombo("Parent", currentModelName))
		{
			for (int n = 0; n < modelNumber; n++)
			{
				bool is_selected = currentModelName == modelNames[n];
				if (ImGui::Selectable(modelNames[n], is_selected))
					currentModelName = modelNames[n];

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (currentModelName != "")
		{
			if (ImGui::Button("Set as parent"))
			{
				std::string name(currentModelName);
				transform.parent = &models[name].transform;
			}
		}
		*/
		ImGui::TreePop();
	}


	for (Component* c : m_components)
	{
		c->drawEditorWindow();
	}

	if (ImGui::TreeNode("OPTIONS"))
	{
		if (ImGui::Button("Add box collider"))
		{
			AddComponent<BoxCollider>();
		}

		if (ImGui::Button("Add sphere collider"))
		{
			AddComponent<SphereCollider>();
		}

		if (ImGui::Button("Add Rigidbody"))
		{
			AddComponent<Rigidbody>();
		}

		if (ImGui::Button("Add Player"))
		{
			AddComponent<Player>();
		}

		ImGui::TreePop();
	}
}


std::string GameObject::getSaveFormat()
{
	std::string s = "gameobject ";
	s += name + "\n";

	s += "    " + transform->getSaveFormat();

	for (int i = 0; i < m_components.size(); ++i)
	{
		s += "    " + m_components[i]->getSaveFormat();
	}

	return s;
}