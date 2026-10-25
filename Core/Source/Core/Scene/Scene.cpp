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

	Actor::Actor* Scene::GetActorByName(std::string name)
	{
		for (uint32_t i = 0; i < GetNumActors(); i++)
		{
			if (GetActors()[i]->GetName() == name)
			{
				return GetActors()[i].get();
			}
		}
		return nullptr;
	}

	Actor::Actor* Scene::GetSelectedActor()
	{
		for (uint32_t i = 0; i < GetNumActors(); i++)
		{
			if (GetActors()[i]->bSelected)
			{
				return GetActors()[i].get();
			}
		}
		return nullptr;
	}

	void Scene::SetSelectedActor(Actor::Actor* actor)
	{
		if (Actor::Actor* selectedActor = GetActorByName(actor->GetName()))
		{
			auto* oldSelectedActor = GetSelectedActor();
			if (oldSelectedActor)
				oldSelectedActor->bSelected = false;
			selectedActor->bSelected = true;
		}
	}
}