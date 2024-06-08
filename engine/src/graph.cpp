#include <graph.h>
using namespace Core::DataStructure;

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <glad/glad.h>

#include <resources_manager.h>

#include <log.h>

#include <input.h>

Graph::Graph()
{
	currentScene = 0;
}

Graph::~Graph()
{
	for (int i = 0; i < scenes.size(); ++i)
	{
		if (scenes[i] != nullptr)
		{
			delete scenes[i];
		}
	}

	scenes.clear();
}

void Graph::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


	if (scenes.empty())
		gameState = GameState::MAIN_MENU;

	int size = (int)scenes.size();

	if (currentScene < 0)
		currentScene = size - 1;
	if (currentScene > size)
		currentScene = 0;

	switch (gameState)
	{
	case GameState::MAIN_MENU:
		if (ImGui::Begin("MAIN MENU"))
		{
			if (ImGui::Button("NEW GAME"))
			{
				gameState = GameState::GAME;
				loadEditScene("resources/scenes/", "defaultScene");
			}
			if (ImGui::Button("CONTINUE"))
			{
				gameState = GameState::GAME;
				loadGameScene("resources/scenes/", "defaultScene");
			}
			if (ImGui::Button("EDIT MODE"))
			{
				gameState = GameState::EDIT;
				loadEditScene("resources/scenes/", "defaultScene");
			}
		}
		ImGui::End();
		break;
	case GameState::EDIT:
		if (ImGui::Begin("PAUSE MENU"))
		{
			if (ImGui::Button("RETURN TO MAIN MENU"))
			{
				gameState = GameState::MAIN_MENU;
				closeScene();
				ImGui::End();
				break;
			}
		}
		ImGui::End();
		showImGuiControls();

		if (currentScene < scenes.size() && scenes[currentScene] != nullptr)
			scenes[currentScene]->updateEditor();
		break;
	case GameState::GAME:
		if (ImGui::Begin("PAUSE MENU"))
		{
			if (ImGui::Button("RETURN TO MAIN MENU"))
			{
				gameState = GameState::MAIN_MENU;
				closeScene();
				ImGui::End();
				break;
			}
		}
		ImGui::End();

		if (currentScene < scenes.size() && scenes[currentScene] != nullptr)
			scenes[currentScene]->update();
		break;
	default:
		break;
	}
}



void Graph::newScene(const std::string& filepath, const std::string& filename)
{
	scenes.push_back(new Scene());
	scenes.back()->filepath = filepath;
	scenes.back()->name = filename;
}

void Graph::loadEditScene(const std::string& filepath, const std::string& filename)
{
	scenes.push_back(new Scene());
	scenes.back()->load_scene(filepath, filename);
}

void Graph::loadGameScene(const std::string& filepath, const std::string& filename)
{
	scenes.push_back(new Scene());
	scenes.back()->load_scene(filepath, filename + "_save");
}

void Graph::saveEditScene(const std::string& filepath, const std::string& filename)
{
	scenes[currentScene]->filepath = filepath;
	scenes[currentScene]->name = filename;
	
	scenes[currentScene]->save_scene(true);
}

void Graph::saveEditScene()
{
	scenes[currentScene]->save_scene(true);
}

void Graph::saveGameScene()
{
	if (gameState == GameState::GAME)
		scenes[currentScene]->save_scene(false);
}

void Graph::closeScene()
{
	if (scenes.empty())
		return;

	Input::instance()->initialize(Input::instance()->window);

	delete scenes[currentScene];
	scenes[currentScene] = scenes.back();
	scenes.pop_back();
	currentScene = 0;
}


void Graph::addModelToCurrentScene(const std::string& filepath, const std::string& filename, const std::string& modelname, const std::string& shadername)
{
	/*
	if (scenes[currentScene].hasModelNamed(modelname))
	{
		Core::Debug::Log::instance()->error("Can't create model because there is already a model with the same name in the current scene !");
		return;
	}*/

	Model model;
	if (model.load_complex_model(filepath, filename, shadername))
	{
		scenes[currentScene]->addModelToScene(ResourcesManager::instance()->add_model(modelname, model));
	}
}


void Graph::showImGuiControls()
{
	ResourcesManager* res = ResourcesManager::instance();

	if (ImGui::Begin("Config"))
	{
		ImGui::Text("(Right click to capture mouse, Esc to un-capture)");
		ImGui::Text("(Make sure to press LEFT ALT while dragging float values to have more precision)");

		if (ImGui::TreeNode("Scene Manager"))
		{
			// NAVIGATE BETWEEN SCENES
			// -----------------------
			int size = (int)scenes.size();
			std::string s = size < 2 ? "" : "s";
			ImGui::Text(("Currently, there is " + std::to_string(size) + " scene" + s + " loaded.").c_str());
			if (!scenes.empty())
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1 });
				ImGui::Text(("Current Scene : " + scenes[currentScene]->name).c_str());
				ImGui::PopStyleColor();

				ImGui::SliderInt(("You have " + std::to_string(size) + " loaded scene.").c_str(), &currentScene, 0, size - 1);
			}

			// LOAD & SAVE SCENE
			// -----------------
			if (ImGui::TreeNode("Scene Load & Save"))
			{
				ImGui::Text("Provide a filepath and a name before creating your scene.");
				ImGui::Text("If you don't save it and close it, you won't be able to get it back.");

				static char scenePath[128] = "resources/scenes/";
				static char sceneName[128] = "test";

				ImGui::InputText("Scene file path", scenePath, IM_ARRAYSIZE(scenePath));
				ImGui::InputText("and scene file name !", sceneName, IM_ARRAYSIZE(sceneName));
				ImGui::Text("------");
				if (ImGui::Button("New Empty Scene"))
				{
					newScene(scenePath, sceneName);
				}
				ImGui::Text("------");
				if (ImGui::Button("Load Edit Scene"))
				{
					loadEditScene(scenePath, sceneName);
				}
				if (ImGui::Button("Load Game Scene"))
				{
					loadEditScene(scenePath, sceneName);
				}
				ImGui::Text("------");
				if (ImGui::Button("Save Edit Scene"))
				{
					saveEditScene();
				}
				if (ImGui::Button("Save Edit Scene as (with new path/name provided)"))
				{
					saveEditScene(scenePath, sceneName);
				}
				if (ImGui::Button("Save Game Scene - TESTING PURPOSES ONLY"))
				{
					saveGameScene();
				}
				ImGui::Text("------");
				if (ImGui::Button("Close scene"))
				{
					closeScene();
				}
				ImGui::Text("------");
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		// If we have no scene loaded, we can't do much
		if (scenes.empty())
		{
			ImGui::End();
			return;
		}

		// LOADING SHADERS TO RESOURCES
		// ----------------------------
		if (ImGui::TreeNode("Shaders"))
		{
			ImGui::Text("---------------------------------------------");
			ImGui::Text("Add a shader by giving its filepath and filename");
			static char shaderPath[128] = "resources/shaders/";
			static char shaderVertex[128] = "advanced_lighting_vs.vert";
			static char shaderFrag[128] = "advanced_lighting_fs.frag";
			static char shaderName[128] = "advanced_lighting_shader";
			ImGui::InputText("Shader file path", shaderPath, IM_ARRAYSIZE(shaderPath));
			ImGui::InputText("vertex file name", shaderVertex, IM_ARRAYSIZE(shaderVertex));
			ImGui::InputText("fragment file name", shaderFrag, IM_ARRAYSIZE(shaderFrag));
			ImGui::InputText("give it a name", shaderName, IM_ARRAYSIZE(shaderName));

			if (ImGui::Button("Add shader"))
			{
				Shader* s = res->get_shader(shaderName);
				if (s == nullptr)
				{
					std::string p = shaderPath;
					std::string v = shaderVertex;
					std::string f = shaderFrag;

					res->add_shader(shaderName, Shader(p + v, p + f));
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GameObject with components"))
		{
			if (ImGui::Button("Add Camera"))
			{
				scenes[currentScene]->addCamToScene();
			}
			if (ImGui::Button("Add Light"))
			{
				scenes[currentScene]->addLightToScene();
			}
			if (ImGui::Button("Add Box Collider"))
			{
				scenes[currentScene]->addBoxCollider();
			}
			if (ImGui::Button("Add Sphere Collider + Rigidbody"))
			{
				scenes[currentScene]->addRigidbody();
			}
			ImGui::TreePop();
		}

		// ADDING MODELS TO SCENE
		// ----------------------
		if (ImGui::TreeNode("GameObject with ModelRenderer"))
		{
			ImGui::Text("-------------------------------");
			ImGui::Text("To add a model, choose a shader");

			const char* shaderNames[16];
			int shaderNumber;
			res->get_shader_names(shaderNames, shaderNumber);
			static const char* currentShader = "";

			ImGui::Text(("Currently, there is " + std::to_string(shaderNumber) + " shader loaded").c_str());

			if (ImGui::BeginCombo("Shader", currentShader))
			{
				for (int n = 0; n < shaderNumber; n++)
				{
					bool is_selected = currentShader == shaderNames[n];
					if (ImGui::Selectable(shaderNames[n], is_selected))
						currentShader = shaderNames[n];

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (currentShader != "")
			{
				ImGui::Text("Add a model by giving its filepath and filename");
				static char objPath[128] = "resources/craftsman/";
				static char objName[128] = "the_noble_craftsman";
				static char modelName[128] = "the_noble_craftsman";

				ImGui::InputText("Model file path", objPath, IM_ARRAYSIZE(objPath));
				ImGui::InputText(".obj file name", objName, IM_ARRAYSIZE(objName));
				ImGui::InputText("Scene model name", modelName, IM_ARRAYSIZE(modelName));

				if (ImGui::Button("Add model"))
				{
					addModelToCurrentScene(objPath, objName, modelName, currentShader);
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();


}