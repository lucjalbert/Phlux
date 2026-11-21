#include "AssetBrowserLayer.h"

#include "Core/Application.h"
#include "Core/Renderer/AssetRegistry/MaterialLibrary.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/AssetRegistry/ModelLibrary.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/ImGui/ImGuiUtils.h"
#include "Core/Renderer/AssetRegistry/AssetRegistry.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

AssetBrowserLayer::AssetBrowserLayer()
	: m_AssetRegistry(Core::Application::Get().GetAssetRegistry())
{
}

void AssetBrowserLayer::OnImGuiRender()
{
	ImGui::Begin("Assets");

	if (ImGui::BeginTabBar("AssetBrowser"))
	{
		if (ImGui::BeginTabItem("Models"))
		{
			bool isFirstModel = true;
			const AssetRegistry::ModelLibrary& modelLibrary = Core::Application::Get().GetAssetRegistry().GetModelLibrary();
			for (AssetRegistry::ModelId modelId : modelLibrary.GetAllModelIds())
			{
				const auto& model = modelLibrary.Get(modelId);

				// We don't want to use SameLine on the first model or the row will stick to the tab button
				if (ImGui::GetContentRegionAvail().x > m_TileSize.x && !isFirstModel)
					ImGui::SameLine();

				Renderer::Texture2D* thumbnail = m_AssetRegistry.GetModelThumbnail(modelId);
				if (thumbnail == nullptr)
					thumbnail = m_AssetRegistry.GenerateAssetThumbnail(modelId, AssetRegistry::MaterialId::Default(), AssetRegistry::AssetType::MODEL);

				ImTextureID thumbnailId = thumbnail->GetID();
				DrawAssetBrowserTile(model.GetName().c_str(), m_TileSize, static_cast<int>(modelId.Value), thumbnailId);

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("ModelId", (void*)&modelId, sizeof(AssetRegistry::ModelId));
					ImGui::Text(model.GetName().c_str());

					ImGui::EndDragDropSource();
				}

				isFirstModel = false;
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{
			bool isFirstMaterial = true;
			const AssetRegistry::MaterialLibrary& materialLibrary = Core::Application::Get().GetAssetRegistry().GetMaterialLibrary();
			for (AssetRegistry::MaterialId materialId : materialLibrary.GetAllMaterialIds())
			{
				const auto& material = materialLibrary.Get(materialId);

				// We don't want to use SameLine on the first material or the row will stick to the tab button
				if (ImGui::GetContentRegionAvail().x > m_TileSize.x && !isFirstMaterial)
					ImGui::SameLine();

				Renderer::Texture2D* thumbnail = m_AssetRegistry.GetMaterialThumbnail(materialId);
				if (thumbnail == nullptr)
					thumbnail = m_AssetRegistry.GenerateAssetThumbnail(AssetRegistry::ModelId(1), materialId, AssetRegistry::AssetType::MATERIAL);

				ImTextureID thumbnailId = thumbnail->GetID();
				DrawAssetBrowserTile(material.GetName().c_str(), m_TileSize, static_cast<int>(materialId.Value), thumbnailId);

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("MaterialId", (void*)&materialId, sizeof(AssetRegistry::MaterialId));
					ImGui::Text(material.GetName().c_str());

					ImGui::EndDragDropSource();
				}

				isFirstMaterial = false;
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

void AssetBrowserLayer::DrawAssetBrowserTile(const char* label, ImVec2 tileSize, int id, ImTextureID thumbnailTextureId)
{
	ImGui::PushID(id);

	bool clicked = ImGui::InvisibleButton(label, tileSize);
	bool hovered = ImGui::IsItemHovered();
	bool active = ImGui::IsItemActive();

	ImVec2 positionMin = ImGui::GetItemRectMin();
	ImVec2 positionMax = ImGui::GetItemRectMax();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImGuiStyle& style = ImGui::GetStyle();

	ImU32 backgroundColor = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
	drawList->AddRectFilled(positionMin, positionMax, backgroundColor, style.FrameRounding);

	float padding = style.FramePadding.x;
	float thumbnailSize = m_TileSize.x - (padding * 2.0f);
	ImVec2 thumbPosMin(positionMin.x + padding, positionMin.y + padding);
	ImVec2 thumbPosMax(thumbPosMin.x + thumbnailSize, thumbPosMin.y + thumbnailSize);
	//drawList->AddRectFilled(thumbPosMin, thumbPosMax, IM_COL32(0, 0, 0, 255));
	drawList->AddImage(thumbnailTextureId, thumbPosMax, thumbPosMin);

	ImVec2 textPosition(positionMin.x + padding, thumbPosMax.y + padding);
	std::string displayText = UI::TruncateTextEllipsis(label, thumbnailSize);

	drawList->PushClipRect(positionMin, positionMax, true);
	drawList->AddText(textPosition, ImGui::GetColorU32(ImGuiCol_Text), displayText.c_str());
	drawList->PopClipRect();

	if (hovered)
		ImGui::SetTooltip("%s", label);

	ImGui::PopID();
}