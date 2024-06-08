#pragma once

#include <vector>

#include <transform.h>
using namespace Physics;

#include <mesh.h>
#include <texture.h>
#include <shader.h>
#include <material.h>
using namespace Resources;

#include <unordered_map>

namespace LowRenderer
{
	class SubModel
	{
	public:
		//Transform transform;
		std::string name;
		bool visible = true;

		Mesh* mesh = nullptr;
		Texture* texture = nullptr;
		Shader* shader = nullptr;
		Material* material = nullptr;

		void setShaderMaterial();
		void draw(const mat4& viewProj, const mat4& modelMatrix);
		
		// IMGUI INTERFACE
		void showImGuiControls();
	};

	class Model
	{
	private:
		bool uniformScaling = true;

	public:
		Model() {};
		~Model() {};

		std::string name;
		std::string filepath;
		std::string shaderName;

		std::vector<SubModel> subModels;

		bool load_complex_model(const std::string& filepath, const std::string& filename, const std::string& shadername);

		void load_mtl(const std::string& filepath, const std::string& filename);

		void draw(const mat4& viewProj, const mat4& modelMatrix);


		// IMGUI INTERFACE
		void showImGuiControls();
	};
}