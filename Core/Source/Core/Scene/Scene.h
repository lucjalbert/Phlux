#pragma once

#include "Actors/Actor.h"

#include <string>
#include <vector>
#include <memory>

namespace Scene {

	class Scene
	{
	public:
		//void Initialize();
		//void Shutdown();
		//bool Open();

		template<typename TActor, typename... Args>
		requires(std::is_base_of_v<Actor::Actor, TActor>)
		void AddActor(Args&&... args)
		{
			m_Actors.push_back(std::make_unique<TActor>(std::forward<Args>(args)...));
		}

		std::vector<std::unique_ptr<Actor::Actor>>& GetActors() { return m_Actors; }
		Actor::Actor* GetActorByName(std::string);
		uint32_t GetNumActors();

		Actor::Actor* GetSelectedActor();
		void SetSelectedActor(Actor::Actor* actor);

		std::string MakeUniqueActorName(std::string name, uint32_t id = 0);

	private:
		std::string m_SourceFile;

		std::vector<std::unique_ptr<Actor::Actor>> m_Actors;
	};
}