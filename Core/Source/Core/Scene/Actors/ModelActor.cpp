#include "ModelActor.h"

#include "Core/Renderer/Model.h"
#include "Core/Renderer/ModelSerialization.h"
#include "Core/Renderer/AssetRegistry/MaterialLibrary.h"
#include "Core/Application.h"
#include "Actor.h"

#include <json.hpp>

#include <memory>

namespace Actor {

	ModelActor::ModelActor(AssetRegistry::ModelId modelId)
		: Actor(Core::Application::Get().GetAssetRegistry().GetModelLibrary().Get(modelId).GetName()),
		m_Model(modelId),
		m_Material(AssetRegistry::MaterialId::Default()),
		m_ModelLibrary(Core::Application::Get().GetAssetRegistry().GetModelLibrary())
	{
	}

	void ModelActor::Render()
	{
		m_ModelLibrary.Get(m_Model).Draw();
	}

	void ModelActor::Serialize(nlohmann::json& j) const
	{
		Actor::Serialize(j);
		j["ModelData"] = ModelToJson(m_ModelLibrary.Get(m_Model));
	}

	void ModelActor::GetProperties(PropertyList& list)
	{
		Actor::GetProperties(list);

		list.Add("Model");
		Renderer::Model& model = Core::Application::Get().GetAssetRegistry().GetModelLibrary().Get(m_Model);
		list.Add("Model", model);
		
		list.Add("Material");
		Renderer::Material& material = Core::Application::Get().GetAssetRegistry().GetMaterialLibrary().Get(m_Material);
		list.Add("Material", material);
		material.GetProperties(list);
	}
}