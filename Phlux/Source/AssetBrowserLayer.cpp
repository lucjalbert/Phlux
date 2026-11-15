#include "AssetBrowserLayer.h"

#include "Core/Application.h"
#include "Core/Renderer/MaterialLibrary.h"

#include <imgui.h>

AssetBrowserLayer::AssetBrowserLayer()
{
}

void AssetBrowserLayer::OnImGuiRender()
{
	ImGui::Begin("Assets");

	if (ImGui::BeginTabBar("AssetBrowser"))
	{
		if (ImGui::BeginTabItem("Models"))
		{
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{
			//int row = 0;
			const Renderer::MaterialLibrary& materialLibrary = Core::Application::Get().GetScene().GetMaterialLibrary();
			for (Renderer::MaterialId materialId : materialLibrary.GetAllMaterialIds())
			{
				ImGui::PushID(static_cast<int>(materialId.Value));

				float elementHeight = ImGui::GetFrameHeight() * 3;

				const auto& material = materialLibrary.Get(materialId);

				if (ImGui::GetContentRegionAvail().x > elementHeight)
					ImGui::SameLine();
				ImGui::Button(material.GetName().c_str(), ImVec2(elementHeight, elementHeight));

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("MaterialId", (void*)&materialId, sizeof(Renderer::MaterialId));
					ImGui::Text(material.GetName().c_str());

					ImGui::EndDragDropSource();
				}

				ImGui::PopID();
			}

			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Textures"))
		{
			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}
	
	ImGui::End();
}