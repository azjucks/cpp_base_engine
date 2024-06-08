#pragma once

#include <vector>

#include <singleton.h>

#include <scene.h>
using namespace Resources;

namespace Core
{
	namespace DataStructure
	{
		enum class GameState
		{
			MAIN_MENU,
			GAME,
			EDIT
		};

		class Graph : public Singleton<Graph>
		{
			friend class Singleton<Graph>;

		private:
			Graph();
			~Graph();

			int currentScene = 0;
			std::vector<Scene*> scenes;

			GameState gameState = GameState::MAIN_MENU;

		public:
			void newScene(const std::string& filepath, const std::string& filename);
			void loadEditScene(const std::string& filepath, const std::string& filename);
			void loadGameScene(const std::string& filepath, const std::string& filename);
			void saveEditScene(const std::string& filepath, const std::string& filename);
			void saveEditScene();
			void saveGameScene();
			void closeScene();

			Scene* getCurrentScene() { return scenes[currentScene]; }

			void update();
			void addModelToCurrentScene(const std::string& filepath, const std::string& filename, const std::string& modelname, const std::string& shadername);

			void showImGuiControls();
		};
	}
}
