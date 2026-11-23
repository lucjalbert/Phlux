#include "ModelLibrary.h"

#include "Core/Renderer/Model.h"
#include "Core/Renderer/ModelSerialization.h"
#include "Core/Utils/JsonUtils.h"

#include <json.hpp>

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

	ModelId ModelLibrary::Add(ModelId id, std::unique_ptr<Renderer::Model> model)
	{
		m_Models.emplace(id.Value, std::move(model));
		return id;
	}

	void ModelLibrary::Destroy(ModelId id)
	{
		if (id.Value != 0) m_Models.erase(id.Value);
	}

	void ModelLibrary::Clear()
	{
		m_Models.clear();
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

		for (const auto& [key, model] : m_Models)
			modelIds.push_back(ModelId{ key });

		std::sort(modelIds.begin(), modelIds.end(),
			[](ModelId a, ModelId b) { return a.Value < b.Value; });

		return modelIds;
	}

	void ModelLibrary::Serialize(nlohmann::json& j) const
	{
		nlohmann::json models = nlohmann::json::array();
		for (ModelId id : GetAllModelIds())
		{
			nlohmann::json model;
			model[JsonKeys::ModelId] = id.Value;
			model[JsonKeys::ModelData] = ModelToJson(Get(id));
			models.push_back(std::move(model));
		}
		j[JsonKeys::ModelLibrary] = std::move(models);
	}

	void ModelLibrary::Deserialize(const nlohmann::json& j)
	{
		for (const auto& idJson : j.at(JsonKeys::ModelLibrary))
		{
			ModelId id = ModelId(idJson.at(JsonKeys::ModelId));
			Add(id, std::make_unique<Renderer::Model>(ModelFromJson(idJson.at(JsonKeys::ModelData))));
		}
	}
}