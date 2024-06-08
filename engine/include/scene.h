#pragma once

#include <camera.h>
#include <light.h>
#include <model.h>
using namespace LowRenderer;

#include <gameobject.h>
using namespace Engine;

#include <physics.h>
using namespace Physics;

namespace Resources
{
	class Scene
	{
	private:
		std::vector<GameObject*> m_gameObjects;
		
		vec3 backgroundColor;

	public:
		std::string filepath;
		std::string name;
		
		Scene();
		~Scene();

		void awake();
		void start();
		void update();
		void updateEditor();

		void awakeGameObjects();
		void startGameObjects();
		void updateGameObjects();
		void fixedUpdateGameObjects();
		void lateUpdateGameObjects();
		void drawCallGameObjects();

		void addModelToScene(Model* model);
		void addCamToScene();
		void addLightToScene();

		void addBoxCollider();
		void addRigidbody();

		GameObject* findGameObjectWithName(const std::string& name);

		void load_scene(const std::string& filepath, const std::string& filename);
		void save_scene(bool isEditSave);

		void showImGuiControls();
	};
}