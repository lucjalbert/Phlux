#include "Actor.h"

#include "Core/Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Actor {

	Actor::Actor(std::string name)
		: m_Name(name)
	{
		m_Name = Core::Application::Get().GetScene()->MakeUniqueActorName(m_Name);
	}

	void Actor::GetProperties(PropertyList& list)
	{
		list.Add("Name", m_Name, true);
		list.Add("Position", m_Position);
		list.Add("Rotation", m_Rotation);
		list.Add("Scale", m_Scale);
	}

	glm::mat4 Actor::GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, m_Position);

		model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, m_Scale);

		return model;
	}
}