#include "DetailPanelLayer.h"

#include "Core/Application.h"
#include "Core/Scene/Actors/Actor.h"
#include "Core/Scene/Actors/ModelActor.h"
#include "Core/Renderer/MaterialLibrary.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <ImGuizmo.h>

#include <string>
#include <iostream>

void DetailPanelLayer::OnImGuiRender()
{
	ImGui::Begin("Detail Panel");

	if (Core::Application::Get().GetScene().GetNumActors() > 0)
	{
		Actor::PropertyList propertyList;

		Actor::Actor* selectedActor = Core::Application::Get().GetScene().GetSelectedActor();
		if (selectedActor)
		{
			selectedActor->GetProperties(propertyList);

			bool IsCategoryOpen = true;

			for (const Actor::Property& property : propertyList.All())
			{
				// Process categories first so we know whether or not to display the following properties
				if (property.Type == Actor::PropertyType::CATEGORY)
				{
					IsCategoryOpen = ImGui::CollapsingHeader(property.Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
					continue;
				}

				if (!IsCategoryOpen)
				{
					continue;
				}

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

				case Actor::PropertyType::MAT4:
					float position[3], rotation[3], scale[3];
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(*static_cast<glm::mat4*>(property.Value)), position, rotation, scale);
					ImGui::DragFloat3("Position", position, m_SliderSpeed);
					ImGui::DragFloat3("Rotation", rotation, m_SliderSpeed); 
					ImGui::DragFloat3("Scale", scale, m_SliderSpeed);
					ImGuizmo::RecomposeMatrixFromComponents(position, rotation, scale, glm::value_ptr(*static_cast<glm::mat4*>(property.Value)));
					break;

				case Actor::PropertyType::MATERIAL:
					SetupMaterialComboMenu("Assigned Material");
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

void DetailPanelLayer::SetupMaterialComboMenu(const char* menuLabel)
{
	const Renderer::MaterialLibrary& materialLibrary = Core::Application::Get().GetScene().GetMaterialLibrary();
	std::vector<Renderer::MaterialId> materials = materialLibrary.GetAllMaterialIds();

	Renderer::MaterialId selectedMaterialId = Renderer::MaterialId::Default();
	auto selectedActor = Core::Application::Get().GetScene().GetSelectedActor();
	auto selectedModelActor = dynamic_cast<Actor::ModelActor*>(selectedActor);
	
	if (!selectedModelActor)
	{
		std::cerr << "Non model actor cannot have a material type property" << std::endl;
		return;
	}

	selectedMaterialId = selectedModelActor->GetMaterialId();

	if (ImGui::BeginCombo(menuLabel, materialLibrary.Get(selectedMaterialId).GetName().c_str()))
	{
		for (Renderer::MaterialId id : materialLibrary.GetAllMaterialIds())
		{
			ImGui::PushID(static_cast<int>(id.Value));

			const bool isSelected = (selectedMaterialId == id);

			if (ImGui::Selectable(materialLibrary.Get(id).GetName().c_str(), isSelected))
			{
				selectedMaterialId = id;
				selectedModelActor->SetMaterial(id);
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();

			ImGui::PopID();
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* droppedMaterialPayload = ImGui::AcceptDragDropPayload("MaterialId"))
		{
			selectedMaterialId = *(Renderer::MaterialId*)droppedMaterialPayload->Data;
			selectedModelActor->SetMaterial(selectedMaterialId);
		}

		ImGui::EndDragDropTarget();
	}
}