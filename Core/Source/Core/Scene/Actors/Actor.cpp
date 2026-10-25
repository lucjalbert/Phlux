#include "Actor.h"

#include "Core/Application.h"

namespace Actor {

	Actor::Actor(std::string name)
		: m_Name(name)
	{
		m_Name = Core::Application::Get().GetScene()->MakeUniqueActorName(m_Name);
	}
}