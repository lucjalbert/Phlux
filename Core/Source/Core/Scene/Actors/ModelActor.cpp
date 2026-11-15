#include "ModelActor.h"

#include "Core/Renderer/Model.h"
#include "Core/Renderer/ModelSerialization.h"
#include "Core/Renderer/MaterialLibrary.h"
#include "Core/Application.h"
#include "Actor.h"

#include <json.hpp>

#include <memory>

namespace Actor {

	ModelActor::ModelActor(Renderer::Model model)
		: Actor(model.GetName()),
		m_Model(std::move(model)),
		m_Material(Renderer::MaterialId::Default())
	{
	}

	void ModelActor::Render()
	{
		m_Model.Draw();
	}

	void ModelActor::Serialize(nlohmann::json& j) const
	{
		Actor::Serialize(j);
		j["ModelData"] = ModelToJson(m_Model);
	}

	void ModelActor::GetProperties(PropertyList& list)
	{
		Actor::GetProperties(list);
		list.Add("Material");
		Renderer::Material& material = Core::Application::Get().GetScene().GetMaterialLibrary().Get(m_Material);
		list.Add("Material", material);
		material.GetProperties(list);
	}
}