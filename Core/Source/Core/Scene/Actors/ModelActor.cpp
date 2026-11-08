#include "ModelActor.h"

#include "Core/Renderer/Model.h"
#include "Core/Renderer/ModelSerialization.h"

#include <json.hpp>

namespace Actor {

	ModelActor::ModelActor(Renderer::Model model)
		: Actor(model.GetName()), m_Model(std::move(model))
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
}