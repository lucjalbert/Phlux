#pragma once

#include "Actors/Actor.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/AssetRegistry/MaterialLibrary.h"
#include "Core/Renderer/AssetRegistry/ModelLibrary.h"

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace Scene {

	class Scene
	{
	public:
		Scene();

		bool Save(std::string destinationPath);
		bool Load(std::filesystem::path source);
		bool Close();

		template<typename TActor, typename... Args>
		requires(std::is_base_of_v<Actor::Actor, TActor>)
		void AddActor(Args&&... args)
		{
			m_Actors.push_back(std::make_unique<TActor>(std::forward<Args>(args)...));
		}

		std::unique_ptr<Actor::Actor> LoadActorFromJson(const nlohmann::json& j);

		std::string MakeUniqueActorName(std::string name, uint32_t id = 0);

		std::vector<std::unique_ptr<Actor::Actor>>& GetActors() { return m_Actors; }
		Actor::Actor* GetActorByName(std::string);
		uint32_t GetNumActors();

		Actor::Actor* GetSelectedActor();
		void SetSelectedActor(Actor::Actor* actor);
		void DeselectActor();
		void SelectActorAtMouseLocation(float mousePosX, float mousePosY);

		bool DeleteActor(Actor::Actor* actor);
		bool DeleteSelectedActor();

		Camera::FlyCamera& GetCamera() { return m_Camera; }
	private:
		std::vector<std::unique_ptr<Actor::Actor>> m_Actors;
		AssetRegistry::ModelLibrary& m_Models;
		AssetRegistry::MaterialLibrary& m_Materials;
		Camera::FlyCamera m_Camera;
	};
}