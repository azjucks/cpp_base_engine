#pragma once

#include <component.h>
using namespace Engine;

#include <model.h>

namespace LowRenderer
{
	class ModelRenderer : public Component
	{
	public:
		ModelRenderer() = default;
		ModelRenderer(GameObject* go)
			: Component(go) {}

		Model* model = nullptr;

		int layer = 0;

		void drawCall() override;

		void drawEditorWindow() override;


		void setModel(Model* m);

		std::string getSaveFormat() override;
		void loadFromSaveFormat(const std::string& line) override;
	};
}

