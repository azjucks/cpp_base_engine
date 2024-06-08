#include <scene.h>

#include <resources_manager.h>

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>
#include <sstream>
#include <string>

#include <log.h>

#include <time_manager.h>
using namespace Core;

#include <renderer.h>
#include <model_renderer.h>
using namespace LowRenderer;

#include <physics.h>
#include <box_collider.h>
#include <sphere_collider.h>
#include <rigidbody.h>
#include <player.h>


Scene::Scene()
{
	filepath = "resources/scenes/";
	name = "sample_scene";

	/*
	/// TEST PURPOSES

	// Add shader
	std::string p = "resources/shaders/";
	std::string v = "advanced_lighting_vs.vert";
	std::string f = "advanced_lighting_fs.frag";
	ResourcesManager::instance()->add_shader("advanced_lighting_shader", Shader(p + v, p + f));

	// Add cam and place it
	addCamToScene();
	Renderer::instance()->cameras[0]->gameObject->transform->position = { 0, 5, 15 };

	// Add directional light
	addLightToScene();
	Renderer::instance()->lights[0]->directional = true;
	Renderer::instance()->lights[0]->gameObject->transform->position = { 10, 10, 10 };

	Model man;
	if (man.load_complex_model("resources/craftsman/", "the_noble_craftsman", "advanced_lighting_shader"))
		addModelToScene(ResourcesManager::instance()->add_model("craftsman", man));

	Model cube;
	if (cube.load_complex_model("resources/", "cube", "advanced_lighting_shader"))
		addModelToScene(ResourcesManager::instance()->add_model("cube", cube));
	*/		
}

Scene::~Scene()
{
	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		if (m_gameObjects[i] != nullptr)
			delete m_gameObjects[i];
	}
	
	m_gameObjects.clear();
}

void Scene::awake()
{
	awakeGameObjects();
}

void Scene::start()
{
	startGameObjects();
}

void Scene::update()
{
	// =========
	// BACKGROUND
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	TimeManager* time = TimeManager::instance();



	


	awake();
	start();

	// =========
	// UPDATE
	updateGameObjects();

	// =========
	// PHYSICS

	time->updateTimePhysics();
	while (time->fixedDeltaLoop())
	{
		fixedUpdateGameObjects();
		PhysicsEngine::instance()->processCollisions();
	}

	// =========
	// RENDERING
	Renderer::instance()->setShaderLights();
	Renderer::instance()->updateViewProjs();
	drawCallGameObjects();
}

void Scene::updateEditor()
{
	// =========
// BACKGROUND
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	TimeManager* time = TimeManager::instance();

	// =========
	// DEBUG
	showImGuiControls();

	awake();
	start();

	// =========
	// UPDATE
	updateGameObjects();

	//time->updateTimePhysics();
	//while (time->fixedDeltaLoop())
	//{
	//	fixedUpdateGameObjects();
	//	PhysicsEngine::instance()->processCollisions();
	//}

	// =========
	// RENDERING
	Renderer::instance()->setShaderLights();
	Renderer::instance()->updateViewProjs();

	drawCallGameObjects();
}






void Scene::awakeGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->awakeComponents();
	}
}
void Scene::startGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->startComponents();
	}
}
void Scene::updateGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->updateComponents();
	}
}
void Scene::fixedUpdateGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->fixedUpdateComponents();
	}
}
void Scene::lateUpdateGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->lateUpdateComponents();
	}
}
void Scene::drawCallGameObjects()
{
	for (GameObject* go : m_gameObjects)
	{
		go->drawCallComponents();
	}
}






void Scene::addModelToScene(Model* model)
{
	m_gameObjects.push_back(new GameObject(model->name + "_GameObject"));
	m_gameObjects.back()->AddComponent<ModelRenderer>()->setModel(model);
}

void Scene::addCamToScene()
{
	m_gameObjects.push_back(new GameObject("Camera_GameObject"));
	m_gameObjects.back()->AddComponent<Camera>();
}

void Scene::addLightToScene()
{
	m_gameObjects.push_back(new GameObject("Light_GameObject"));
	m_gameObjects.back()->AddComponent<Light>();
}

void Scene::addBoxCollider()
{
	m_gameObjects.push_back(new GameObject("Box_Collider_Gameobject"));
	m_gameObjects.back()->AddComponent<BoxCollider>();
}

void Scene::addRigidbody()
{
	m_gameObjects.push_back(new GameObject("Rigidbody_Gameobject"));
	m_gameObjects.back()->AddComponent<SphereCollider>();
	m_gameObjects.back()->AddComponent<Rigidbody>();
}

GameObject* Scene::findGameObjectWithName(const std::string& name)
{
	for (GameObject* go : m_gameObjects)
	{
		if (go->name == name)
		{
			return go;
		}
	}
	return nullptr;
}

void Scene::load_scene(const std::string& filepath, const std::string& filename)
{
	Core::Debug::Log::instance()->start_load_scene(filepath, filename);

	ResourcesManager* res = ResourcesManager::instance();

	this->filepath = filepath;
	this->name = filename;

	std::string filefullpath = filepath + filename + ".scene";

	std::ifstream file;
	file.open(filefullpath);
	if (!file.good())
	{
		Core::Debug::Log::instance()->cannot_open_file(filefullpath);
		Core::Debug::Log::instance()->success_load_scene(false);
		return;
	}

	std::string line;
	while (getline(file, line))
	{
		std::istringstream iss(line);

		std::string text;
		iss >> text;

		if (text == "bckg")
		{
			iss >> backgroundColor.x;
			iss >> backgroundColor.y;
			iss >> backgroundColor.z;
		}

		if (text == "shader")
		{
			std::string shadername;
			std::string vertexPath;
			std::string fragmentPath;
			iss >> shadername;
			iss >> vertexPath;
			iss >> fragmentPath;

			Shader shader(vertexPath, fragmentPath);
			res->add_shader(shadername, shader);
		}

		if (text == "gameobject")
		{
			std::string name;
			iss >> name;
			m_gameObjects.push_back(new GameObject(name));
		}

		if (text == "transform")
		{
			m_gameObjects.back()->transform->loadFromSaveFormat(line);
		}

		if (text == "component")
		{
			int type;
			iss >> type;

			switch ((ComponentType)type)
			{
			case ComponentType::CAMERA:
				m_gameObjects.back()->AddComponent<Camera>()->loadFromSaveFormat(line);
				break;
			case ComponentType::LIGHT:
				m_gameObjects.back()->AddComponent<Light>()->loadFromSaveFormat(line);
				break;
			case ComponentType::MODEL_RENDERER:
				m_gameObjects.back()->AddComponent<ModelRenderer>()->loadFromSaveFormat(line);
				break;
			case ComponentType::RIGIDBODY:
				m_gameObjects.back()->AddComponent<Rigidbody>()->loadFromSaveFormat(line);
				break;
			case ComponentType::COLLIDER_BOX:
				m_gameObjects.back()->AddComponent<BoxCollider>()->loadFromSaveFormat(line);
				break;
			case ComponentType::COLLIDER_SPHERE:
				m_gameObjects.back()->AddComponent<SphereCollider>()->loadFromSaveFormat(line);
				break;
			case ComponentType::PLAYER:
				m_gameObjects.back()->AddComponent<Player>()->loadFromSaveFormat(line);
				break;
			default:
				break;
			}
		}
	}

	file.close();

	Core::Debug::Log::instance()->success_load_scene(true);
}

void Scene::save_scene(bool isEditSave)
{
	Core::Debug::Log::instance()->saving_scene(filepath, name);

	ResourcesManager* res = ResourcesManager::instance();
	Renderer* renderer = Renderer::instance();

	std::string filename = filepath + name + (isEditSave ? "" : "_save") + ".scene";

	std::ofstream file(filename);
	if (!file.good())
	{
		Core::Debug::Log::instance()->success_saving_scene(false);
		return;
	}

	file << "bckg ";
	file << backgroundColor.x << " ";
	file << backgroundColor.y << " ";
	file << backgroundColor.z;
	file << "\n";

	// saving all shaders
	for (std::unordered_map<std::string, Shader>::iterator it = res->shaders.begin(); it != res->shaders.end(); it++)
	{
		file << "shader ";
		file << it->first << " ";
		file << it->second.vertexPath << " ";
		file << it->second.fragmentPath;
		file << "\n";
	}


	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		GameObject* go = m_gameObjects[i];

		file << go->getSaveFormat();
	}

	file.close();

	Core::Debug::Log::instance()->success_saving_scene(true);
}



void Scene::showImGuiControls()
{
	
	ResourcesManager* res = ResourcesManager::instance();
	if (ImGui::Begin("Current Scene Parameters"))
	{
		ImGui::ColorEdit3("Background Color", backgroundColor.e);

		if (ImGui::TreeNode("Hierarchy"))
		{
			if (m_gameObjects.size() <= 0)
			{

			}
			else
			{
				static int currentGo = 0;
				if (currentGo > m_gameObjects.size() - 1)
					currentGo -= 1;
				if (currentGo < 0)
					currentGo = 0;

				std::string goNb = "There is currently " + std::to_string(m_gameObjects.size()) + " GameObject(s) in the scene";
				ImGui::Text(goNb.c_str());

				ImGui::SliderInt(m_gameObjects[currentGo]->name.c_str(), &currentGo, 0, (int)m_gameObjects.size() - 1);

				m_gameObjects[currentGo]->drawEditorWindow();

				static bool wantToDelete = false;

				if (wantToDelete || ImGui::Button("Delete GameObject ?"))
				{
					wantToDelete = true;

					ImGui::Text("Are you sure ?");
					if (ImGui::Button("Yes"))
					{
						delete m_gameObjects[currentGo];
						m_gameObjects[currentGo] = m_gameObjects.back();
						m_gameObjects.pop_back();
						wantToDelete = false;
					}
					if (ImGui::Button("No"))
					{
						wantToDelete = false;
					}
				}
			}

			ImGui::TreePop();
		}


		/*
		static int currentModel = 0;
		if (currentModel > models.size() - 1)
			currentModel = 0;

		const char* modelNames[32];
		get_model_names(modelNames);

		if (models.size() > 0 && ImGui::TreeNode("Models"))
		{
			ImGui::Text("Choose a model to edit :");
			ImGui::SliderInt("Model n¤", &currentModel, 0, (int)models.size() - 1);
			ImGui::PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1 });
			ImGui::Text(modelNames[currentModel]);
			ImGui::PopStyleColor();
			models[modelNames[currentModel]].showImGuiControls(models, modelNames, (int)models.size());

			static bool ready = false;
			if (ImGui::Button("Delete ?"))
			{
				ready = true;
			}
			if (ready)
			{
				if (ImGui::Button("Yes"))
				{
					std::cout << "Model " + models[modelNames[currentModel]].name + " deleted from scene." << std::endl;
					models.erase(modelNames[currentModel]);
					currentModel = 0;
					ready = false;
				}
				if (ImGui::Button("No"))
				{
					ready = false;
				}
			}
			ImGui::TreePop();
		}
		*/
	}
	ImGui::End();
}