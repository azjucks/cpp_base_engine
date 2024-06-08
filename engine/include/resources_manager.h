#pragma once

#include <unordered_map>

#include <iostream>
using namespace std;

#include <singleton.h>

#include <mesh.h>
#include <texture.h>
#include <shader.h>
#include <scene.h>
#include <material.h>
using namespace Resources;

namespace Resources
{
	class ResourcesManager : public Singleton<ResourcesManager>
	{
		friend class Singleton<ResourcesManager>;
	
	private:
		ResourcesManager() = default;
		~ResourcesManager() {}

	public:
		std::unordered_map<std::string, Mesh> meshes;
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<std::string, Shader> shaders;
		std::unordered_map<std::string, Material> materials;
		std::unordered_map<std::string, Model> models;

		Mesh* add_mesh(std::string name, Mesh mesh);
		Texture* add_texture(std::string name, Texture texture);
		Shader* add_shader(std::string name, Shader shader);
		Material* add_material(std::string name, Material material);
		Model* add_model(std::string name, Model model);

		Mesh* get_mesh(const std::string& name);
		Texture* get_texture(const std::string& name);
		Shader* get_shader(const std::string& name);
		Material* get_material(const std::string& name);
		Model* get_model(const std::string& name);

		void get_shader_names(const char** nameList, int& size);

		void delete_mesh(std::string name);
		void delete_texture(std::string name);
		void delete_shader(std::string name);
		void delete_material(std::string name);
		void delete_model(std::string name);
	};

}