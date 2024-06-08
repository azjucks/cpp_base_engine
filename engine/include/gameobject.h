#pragma once

#include <string>
#include <vector>

#include <memory>

#include <component.h>

#include <transform.h>
using namespace Physics;

#include <fstream>
#include <sstream>


namespace Engine
{
	class GameObject
	{

	protected:
		std::vector<Component*> m_components;

	public:
		GameObject(std::string n = "GameObject") : name(n), transform(std::make_unique<Transform>(this)) {}
		~GameObject();

		std::string name = "";
		std::unique_ptr<Transform> transform;

		template <typename C>
		C* AddComponent()
		{
			m_components.push_back(new C(this));
			return (C*)m_components.back();
		}

		template <typename C>
		C* GetComponent()
		{
			for (Component* compo : m_components)
			{
				C* castCompo = dynamic_cast<C*>(compo);

				if (castCompo != nullptr)
				{
					return castCompo;
				}
			}
			
			return nullptr;
		}

		void RemoveComponent(Component* c)
		{
			for (int i = 0; i < m_components.size(); ++i)
			{
				if (m_components[i] == c)
				{
					m_components[i] = m_components.back();
					m_components.pop_back();
					return;
				}
			}
		}

		void awakeComponents();
		void startComponents();
		void updateComponents();
		void fixedUpdateComponents();
		void lateUpdateComponents();
		void drawCallComponents();

		void drawEditorWindow();

		std::string getSaveFormat();
	};
}