#include "ModelLibrary.h"

#include "Core/Renderer/Model.h"

#include <string>
#include <memory>
#include <iostream>

namespace AssetRegistry {

	ModelLibrary::ModelLibrary()
	{
		Create("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\Cube.obj");
		Create("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\Sphere.obj");
		Create("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\Cone.obj");
		Create("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\Susan.obj");
	}

	ModelId ModelLibrary::Create(std::string path)
	{
		ModelId id{ m_NextId++ };
		m_Models.emplace(id.Value, std::make_unique<Renderer::Model>(path));
		return id;
	}

	void ModelLibrary::Destroy(ModelId id)
	{
		if (id.Value != 0) m_Models.erase(id.Value);
	}

	Renderer::Model& ModelLibrary::Get(ModelId id)
	{
		auto model = m_Models.find(id.Value);
		if (model != m_Models.end())
		{
			return *model->second;
		}
		std::cerr << "Invalid map value" << std::endl;
		return *model->second;
	}

	const Renderer::Model& ModelLibrary::Get(ModelId id) const
	{
		auto model = m_Models.find(id.Value);
		if (model != m_Models.end())
		{
			return *model->second;
		}
		std::cerr << "Invalid map value" << std::endl;
		return *model->second;
	}

	std::vector<ModelId> ModelLibrary::GetAllModelIds() const
	{
		std::vector<ModelId> modelIds;
		modelIds.reserve(m_Models.size());

		for (const auto& [key, material] : m_Models)
			modelIds.push_back(ModelId{ key });

		std::sort(modelIds.begin() + 1, modelIds.end(),
			[](ModelId a, ModelId b) { return a.Value < b.Value; });

		return modelIds;
	}
}