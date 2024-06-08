#include <resources_manager.h>
using namespace Resources;

Mesh* ResourcesManager::add_mesh(std::string name, Mesh mesh)
{
	meshes[name] = mesh;
	return &meshes[name];
}

Texture* ResourcesManager::add_texture(std::string name, Texture texture)
{
	textures[name] = texture;
	return &textures[name];
}

Shader* ResourcesManager::add_shader(std::string name, Shader shader)
{
	shaders[name] = shader;
	return &shaders[name];
}

Material* ResourcesManager::add_material(std::string name, Material material)
{
	materials[name] = material;
	return &materials[name];
}

Model* ResourcesManager::add_model(std::string name, Model model)
{
	models[name] = model;
	return &models[name];
}


Mesh* ResourcesManager::get_mesh(const std::string& name)
{
	for (auto& mesh : meshes)
	{
		if (name == mesh.first)
		{
			return &mesh.second;
		}
	}

	return nullptr;
}

Texture* ResourcesManager::get_texture(const std::string& name)
{
	for (auto& texture : textures)
	{
		if (name == texture.first)
		{
			return &texture.second;
		}
	}

	return nullptr;
}

Shader* ResourcesManager::get_shader(const std::string& name)
{
	for (auto& shader : shaders)
	{
		if (name == shader.first)
		{
			return &shader.second;
		}
	}

	return nullptr;
}

Material* ResourcesManager::get_material(const std::string& name)
{
	for (auto& material : materials)
	{
		if (name == material.first)
		{
			return &material.second;
		}
	}

	return nullptr;
}

Model* ResourcesManager::get_model(const std::string& name)
{
	for (auto& model : models)
	{
		if (name == model.first)
		{
			return &model.second;
		}
	}

	return nullptr;
}

void ResourcesManager::get_shader_names(const char** nameList, int& size)
{
	int i = 0;
	for (auto& shader : shaders)
	{
		nameList[i++] = shader.first.c_str();	
	}
	size = (int)shaders.size();
}


void ResourcesManager::delete_mesh(std::string name)
{
	meshes.erase(name);
}

void ResourcesManager::delete_texture(std::string name)
{
	textures.erase(name);
}
void ResourcesManager::delete_shader(std::string name)
{
	shaders.erase(name);
}
void ResourcesManager::delete_material(std::string name)
{
	materials.erase(name);
}
void ResourcesManager::delete_model(std::string name)
{
	models.erase(name);
}