#include "OutlinerLayer.h"

#include "Core/Application.h"
#include "Core/Scene/Scene.h"

#include <imgui.h>

#include <memory>

void OutlinerLayer::OnImGuiRender()
{
	std::shared_ptr<Scene::Scene> scene = Core::Application::Get().GetScene();
	
	ImGui::Begin("Outliner");

	static int selected = -1;
	for (uint32_t i = 0; i < scene->GetNumActors(); i++)
	{
		std::string actorName = scene->GetActors()[i]->GetName();
		if (actorName.empty())
		{
			actorName = "Object";
		}

		if (ImGui::Selectable(actorName.c_str(), selected == i))
		{
			scene->SetSelectedActor(scene->GetActors()[i].get());
			selected = i;
		}
	}

	ImGui::End();
}