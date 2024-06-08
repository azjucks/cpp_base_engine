#pragma once


#include <sstream>
#include <string>

namespace Engine
{
	enum class ComponentType
	{
		CAMERA,
		LIGHT,
		MODEL_RENDERER,
		RIGIDBODY,
		COLLIDER_BOX,
		COLLIDER_SPHERE,
		PLAYER,

		COUNT
	};

	class GameObject;

	class Component
	{
	protected:
		bool m_enabled = true;


	public:
		GameObject* gameObject;

		bool hasAwaken = false;
		bool hasStarted = false;

		Component(GameObject* go)
			: gameObject(go) { }
		virtual ~Component();


		void virtual awake() { }
		void virtual start() { }
		void virtual update() { }
		void virtual fixedUpdate() { }
		void virtual lateUpdate() { }

		void virtual drawCall() { }

		void virtual onTriggerEnter() { }
		void virtual onTriggerStay() { }
		void virtual onCollision() { }

		void setActive(bool value);

		void virtual onEnable() { }
		void virtual onDisable() { }

		void virtual drawEditorWindow();

		std::string virtual getSaveFormat() { return ""; }
		void virtual loadFromSaveFormat(const std::string& line) { }
	};
}