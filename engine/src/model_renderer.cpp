#include <model_renderer.h>
#include <renderer.h>
using namespace LowRenderer;

#include <gameobject.h>


void ModelRenderer::drawCall()
{
	model->draw(Renderer::instance()->getViewProj(layer), gameObject->transform->modelMatrix);
}

void ModelRenderer::setModel(Model* m)
{
	model = m;
}

#include <imgui.h>

void ModelRenderer::drawEditorWindow()
{
	if (ImGui::TreeNode("Component: ModelRenderer"))
	{
		model->showImGuiControls();

		ImGui::TreePop();
	}
}

#include <resources_manager.h>

std::string ModelRenderer::getSaveFormat()
{
	std::string s = "component " + std::to_string((int)ComponentType::MODEL_RENDERER) + " "
		+ model->filepath + " "
		+ model->name + " "
		+ model->shaderName + " ";

	for (int j = 0; j < model->subModels.size(); ++j)
	{
		s += "sub "
			+ model->subModels[j].name + " "
			+ std::to_string(model->subModels[j].visible) + " "
			+ "mat "
			+ std::to_string(model->subModels[j].material->ambient.x) + " "
			+ std::to_string(model->subModels[j].material->ambient.y) + " "
			+ std::to_string(model->subModels[j].material->ambient.z) + " "
			+ std::to_string(model->subModels[j].material->diffuse.x) + " "
			+ std::to_string(model->subModels[j].material->diffuse.y) + " "
			+ std::to_string(model->subModels[j].material->diffuse.z) + " "
			+ std::to_string(model->subModels[j].material->specular.x) + " "
			+ std::to_string(model->subModels[j].material->specular.y) + " "
			+ std::to_string(model->subModels[j].material->specular.z) + " "
			+ std::to_string(model->subModels[j].material->emissive.x) + " "
			+ std::to_string(model->subModels[j].material->emissive.y) + " "
			+ std::to_string(model->subModels[j].material->emissive.z) + " "
			+ std::to_string(model->subModels[j].material->shininess) + " ";
	}

	s += "end ";
	s += "\n";

	return s;
}

void ModelRenderer::loadFromSaveFormat(const std::string& line)
{
	ResourcesManager* res = ResourcesManager::instance();

	std::istringstream iss(line);

	std::string component;
	iss >> component;
	iss >> component;

	std::string filepath;
	std::string filename;
	std::string shadername;

	iss >> filepath;
	iss >> filename;
	iss >> shadername;

	Shader* shader = res->get_shader(shadername);

	Model m;
	if (m.load_complex_model(filepath, filename, shadername))
	{
		std::string currentSubName;
		int currentSub = -1;

		std::string s;
		iss >> s;
		while (s != "end")
		{
			if (s == "sub")
			{
				currentSub++;
				iss >> currentSubName;
				iss >> m.subModels[currentSub].visible;
				iss >> s;
				if (s == "mat")
				{
					Material* mat = res->get_material(currentSubName);

					if (mat != nullptr)
					{
						iss >> mat->ambient.x;
						iss >> mat->ambient.y;
						iss >> mat->ambient.z;
						iss >> mat->diffuse.x;
						iss >> mat->diffuse.y;
						iss >> mat->diffuse.z;
						iss >> mat->specular.x;
						iss >> mat->specular.y;
						iss >> mat->specular.z;
						iss >> mat->emissive.x;
						iss >> mat->emissive.y;
						iss >> mat->emissive.z;
						iss >> mat->shininess;
					}
				}
			}

			s = "";
			iss >> s;
		}

		model = ResourcesManager::instance()->add_model(filename, m);
	}
}