#include <model.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

using namespace std;
#include <fstream>
#include <sstream>
#include <string>

#include <resources_manager.h>
using namespace Resources;

#include <log.h>

void LowRenderer::SubModel::setShaderMaterial()
{
	shader->setVec3("material.ambient", material->ambient);
	shader->setVec3("material.diffuse", material->diffuse);
	shader->setVec3("material.specular", material->specular);
	shader->setVec3("material.emissive", material->emissive);
	shader->setFloat("material.shininess", material->shininess);
}

void LowRenderer::SubModel::draw(const mat4& viewProj, const mat4& modelMatrix)
{
	if (!visible)
		return;

	setShaderMaterial();

	shader->use();

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->ID);

	GLint uniModel = glGetUniformLocation(shader->ID, "model");
	if (uniModel == -1)
	{
		std::cout << "Unable to find model" << std::endl;
	}
	GLint uniViewProj = glGetUniformLocation(shader->ID, "viewProj");
	if (uniViewProj == -1)
	{
		std::cout << "Unable to find viewProj" << std::endl;
	}

	glUniformMatrix4fv(uniViewProj, 1, GL_FALSE, viewProj.e);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, modelMatrix.e);

	glBindVertexArray(mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->vertices.size());
}

void LowRenderer::SubModel::showImGuiControls()
{
	ImGui::Checkbox("Visible", &visible);

	if (!visible)
		return;

	ImGui::ColorEdit3("Material ambient", material->ambient.e);
	ImGui::ColorEdit3("Material diffuse", material->diffuse.e);
	ImGui::ColorEdit3("Material specular", material->specular.e);
	ImGui::ColorEdit3("Material emissive", material->emissive.e);
	ImGui::SliderFloat("Material shininess", &material->shininess, 0, 128);
}


bool LowRenderer::Model::load_complex_model(const std::string& filepath, const std::string& filename, const std::string& shadername)
{
	Shader* shader = ResourcesManager::instance()->get_shader(shadername);

	Core::Debug::Log::instance()->start_load_obj(filepath, filename);

	ResourcesManager* res = ResourcesManager::instance();

	this->filepath = filepath;
	this->name = filename;
	this->shaderName = shadername;

	ifstream file;
	file.open(filepath + filename + ".obj");
	if (!file.good())
	{
		Core::Debug::Log::instance()->error("Can't open obj file !");
		Core::Debug::Log::instance()->success_load_obj(false);
		return false;
	}

	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> uvs;
	vector<Vertex> vertices;

	Mesh mesh;

	bool isFirst = true;

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);

		string text = "";
		iss >> text;

		if (text == "mtllib")
		{
			string mtlPath;
			iss >> mtlPath;

			load_mtl(filepath, mtlPath.c_str());
		}

		if (text == "o" || text == "g")
		{
			string meshName;
			iss >> meshName;

			SubModel sub;
			sub.shader = shader;
			sub.name = meshName;
			res->add_material(meshName, Material());
			sub.material = res->get_material(sub.name);

			Mesh* loadedMesh = res->get_mesh(meshName);

			if (loadedMesh != nullptr)
			{
				sub.mesh = loadedMesh;
				subModels.push_back(sub);
				mesh = Mesh();
			}
			else if (isFirst)
			{
				subModels.push_back(sub);
				isFirst = false;
				mesh = Mesh();
			}
			else
			{
				mesh.setUpBuffer();
				res->add_mesh(subModels.back().name, mesh);
				subModels.back().mesh = res->get_mesh(subModels.back().name);

				subModels.push_back(sub);
				mesh = Mesh();
			}
		}

		if (text == "usemtl") // Link texture to our submesh
		{
			iss >> text;
			Texture* t = res->get_texture(text);
			if (t)
			{
				subModels.back().texture = t;
			}
			else
			{
				Core::Debug::Log::instance()->error("Texture required by obj has not been loaded!");
			}
		}

		if (text == "v")
		{
			vec3 pos = { 0 };
			iss >> pos.x;
			iss >> pos.y;
			iss >> pos.z;
			positions.push_back(pos);
		}

		if (text == "vn")
		{
			vec3 normal = { 0 };
			iss >> normal.x;
			iss >> normal.y;
			iss >> normal.z;
			normals.push_back(normal);
		}

		if (text == "vt")
		{
			vec2 uv = { 0 };
			iss >> uv.x;
			iss >> uv.y;
			uvs.push_back(uv);
		}

		if (text == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				unsigned int id[3] = { 0 };

				for (int j = 0; j < 3; ++j)
				{
					iss >> id[j];
					iss.ignore();
				}

				Vertex v = { 0 };
				v.position = positions[id[0] - 1];
				v.normal = normals[id[2] - 1];
				v.uv = uvs[id[1] - 1];

				mesh.vertices.push_back(v);
			}
		}
	}

	subModels.back().shader = shader;
	res->add_material(subModels.back().name, Material());
	subModels.back().material = res->get_material(subModels.back().name);

	mesh.setUpBuffer();
	res->add_mesh(subModels.back().name, mesh);
	subModels.back().mesh = res->get_mesh(subModels.back().name);

	Core::Debug::Log::instance()->success_load_obj(true);
	return true;
}

void LowRenderer::Model::load_mtl(const std::string& filepath, const std::string& filename)
{
	ifstream file;
	file.open(filepath + filename);
	if (!file.good())
	{
		Core::Debug::Log::instance()->cannot_open_file(filename + ".mtl");
		return;
	}

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);

		string text;
		iss >> text;
		if (text == "newmtl")
		{
			iss >> text;

			std::string name = text;

			while (getline(file, line))
			{
				if (ResourcesManager::instance()->get_texture(name))
					break;

				istringstream ss(line);

				string word;
				ss >> word;

				if (word == "map_Kd")
				{
					ss >> word;
					ResourcesManager::instance()->add_texture(name, Texture(filepath + word, true));
					break;
				}
			}
		}
	}
}

void LowRenderer::Model::draw(const mat4& viewProj, const mat4& modelMatrix)
{
	for (int i = 0; i < subModels.size(); ++i)
	{
		subModels[i].draw(viewProj, modelMatrix);
	}
}

void LowRenderer::Model::showImGuiControls()
{
	if (ImGui::TreeNode("SubMeshes"))
	{
		if (subModels.size() <= 0)
		{
			ImGui::Text("No submeshes");
			ImGui::TreePop();
			return;
		}

		static int currentSub = 0;
		if (currentSub > subModels.size() - 1)
			currentSub = 0;

		ImGui::Text("Choose a subMesh to edit :");
		ImGui::SliderInt(subModels[currentSub].name.c_str(), &currentSub, 0, (int)subModels.size() - 1);

		//ImGui::Text(("SubMesh " + std::to_string(currentSub)).c_str());
		subModels[currentSub].showImGuiControls();
		ImGui::TreePop();
	}
}