#include "Scene.h"

#include "Core/Application.h"
#include "Actors/Actor.h"
#include "Actors/ModelActor.h"
#include "Core/Utils/RayCast.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Mesh.h"

#include <glm/glm.hpp>

#include <string>
#include <format>
#include <limits>

namespace Scene {

	Scene::Scene()
	{
		Camera::FlyCameraSpecification flyCameraSpec;
		m_Camera = std::make_shared<Camera::FlyCamera>(Camera::FlyCamera(flyCameraSpec));
	}

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

	void Scene::DeselectActor()
	{
		if (auto* selectedActor = GetSelectedActor())
		{
			selectedActor->bSelected = false;
		}
	}

	void Scene::SelectActorAtMouseLocation(float mousePosX, float mousePosY)
	{
		glm::vec2 windowSize = Core::Application::Get().GetFramebufferSize();

		Utils::Ray worldSpaceRay = Utils::GenerateRayFromMousePos(mousePosX, mousePosY, windowSize.x, windowSize.y, m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix());

		float smallestHitDistance = std::numeric_limits<float>::infinity();
		Actor::Actor* actorToSelect = nullptr;

		for (uint32_t i = 0; i < GetNumActors(); i++)
		{
			auto* modelActor = dynamic_cast<Actor::ModelActor*>(GetActors()[i].get());
			if (!modelActor)
				continue;

			glm::mat4 invModel = glm::inverse(modelActor->GetModelMatrix());
			
			Utils::Ray localSpaceRay;
			localSpaceRay.origin = glm::vec3(invModel * glm::vec4(worldSpaceRay.origin, 1.0f));
			localSpaceRay.direction = glm::vec3(invModel * glm::vec4(worldSpaceRay.direction, 0.0f));

			const auto& meshes = modelActor->GetModel().GetMeshes();
			for (const auto& mesh : meshes)
			{
				const auto& indices = mesh.GetIndices();
				const auto& vertices = mesh.GetVertices();
				for (uint32_t k = 0; k < indices.size(); k += 3)
				{
					float hitDistance;
					if (Utils::RayTriangleIntersect(localSpaceRay.origin, localSpaceRay.direction, vertices[indices[k]].Position, vertices[indices[k + 1]].Position, vertices[indices[k + 2]].Position, hitDistance))
					{
						if (hitDistance < smallestHitDistance)
						{
							smallestHitDistance = hitDistance;
							actorToSelect = modelActor;
						}
					}
				}
			}
		}

		if (actorToSelect != nullptr)
		{
			SetSelectedActor(actorToSelect);
		}
		else
		{
			DeselectActor();
		}
	}
}