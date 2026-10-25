#include "Scene.h"

#include "Actors/Actor.h"

#include <string>
#include <format>

namespace Scene {

	uint32_t Scene::GetNumActors()
	{
		return m_Actors.size();
	}

	std::string Scene::MakeUniqueActorName(std::string name, uint32_t id)
	{
		std::string uniqueName = std::format("{}{}", name, id);
		for (uint32_t i = 0; i < m_Actors.size(); i++)
		{
			if (m_Actors[i]->GetName() == uniqueName)
			{
				uniqueName = Scene::MakeUniqueActorName(name, id + 1);
			}
		}

		return uniqueName;
	}
}