#include "Actor.h"

#include "Core/Application.h"
#include "Core/Utils/JsonUtils.h"
#include "Core/Scene/Actors/ModelActor.h"
#include "Core/Renderer/ModelSerialization.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <json.hpp>

namespace Actor {

	Actor::Actor(std::string name)
		: m_Name(name)
	{
		m_Name = Core::Application::Get().GetScene().MakeUniqueActorName(m_Name);
	}

	void Actor::GetProperties(PropertyList& list)
	{
		list.Add("Name", m_Name, true);
		list.Add("Transform", m_ModelMatrix);
	}

	void Actor::Serialize(nlohmann::json& j) const
	{
		j[JsonKeys::ActorType] = GetTypeName();
		j[JsonKeys::ActorName] = m_Name;
		j[JsonKeys::ActorTransform] = m_ModelMatrix;
	}

	void Actor::Deserialize(const nlohmann::json& j)
	{
		m_Name = j.at(JsonKeys::ActorName).get<std::string>();
		m_ModelMatrix = j.at(JsonKeys::ActorTransform).get<glm::mat4>();
	}
}