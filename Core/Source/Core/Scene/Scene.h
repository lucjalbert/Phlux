#pragma once

#include "Actors/Actor.h"
#include "Core/Camera/FlyCamera.h"

#include <string>
#include <vector>
#include <memory>

namespace Scene {

	class Scene
	{
	public:
		Scene();

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
		void DeselectActor();

		void SelectActorAtMouseLocation(float mousePosX, float mousePosY);

		std::string MakeUniqueActorName(std::string name, uint32_t id = 0);

		std::shared_ptr<Camera::FlyCamera> GetCamera() const { return m_Camera; }
	private:
		std::vector<std::unique_ptr<Actor::Actor>> m_Actors;
		std::shared_ptr<Camera::FlyCamera> m_Camera;
	};
}