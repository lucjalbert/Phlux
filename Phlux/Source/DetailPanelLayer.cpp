#include "DetailPanelLayer.h"

#include "Core/Application.h"
#include "Core/Scene/Actors/Actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <string>

void DetailPanelLayer::OnImGuiRender()
{
	ImGui::Begin("Detail Panel");

	if (Core::Application::Get().GetScene()->GetNumActors() > 0)
	{
		Actor::PropertyList propertyList;

		Actor::Actor* selectedActor = Core::Application::Get().GetScene()->GetSelectedActor();
		if (selectedActor)
		{
			selectedActor->GetProperties(propertyList);

			for (const Actor::Property& property : propertyList.All())
			{
				ImGui::BeginDisabled(property.bReadOnly);

				switch (property.Type)
				{
				case Actor::PropertyType::BOOL:
					ImGui::Checkbox(property.Name.c_str(), static_cast<bool*>(property.Value));
					break;
				case Actor::PropertyType::STRING:
					ImGui::InputText(property.Name.c_str(), static_cast<std::string*>(property.Value));
					break;
				case Actor::PropertyType::INT:
					ImGui::DragInt(property.Name.c_str(), static_cast<int*>(property.Value), m_SliderSpeed);
					break;
				case Actor::PropertyType::FLOAT:
					ImGui::DragFloat(property.Name.c_str(), static_cast<float*>(property.Value), m_SliderSpeed);
					break;
				case Actor::PropertyType::VEC2:
					ImGui::DragFloat2(property.Name.c_str(), glm::value_ptr(*static_cast<glm::vec2*>(property.Value)), m_SliderSpeed);
					break;
				case Actor::PropertyType::VEC3:
					ImGui::DragFloat3(property.Name.c_str(), glm::value_ptr(*static_cast<glm::vec3*>(property.Value)), m_SliderSpeed);
					break;
				case Actor::PropertyType::VEC4:
					ImGui::DragFloat4(property.Name.c_str(), glm::value_ptr(*static_cast<glm::vec4*>(property.Value)), m_SliderSpeed);
					break;
				default:
					break;
				}

				ImGui::EndDisabled();
			}
		}
	}

	ImGui::End();
}