#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/AssetRegistry/AssetRegistry.h"

#include <glm/glm.hpp>

#include <imgui.h>

#include <cstdint>

class AssetBrowserLayer : public Core::Layer
{
public:
	AssetBrowserLayer();

	void OnImGuiRender() override;

private:
	AssetRegistry::AssetRegistry& m_AssetRegistry;

	ImVec2 m_TileSize = ImVec2(100.0f, 120.0f);

	void DrawAssetBrowserTile(const char* label, ImVec2 tileSize, int id, ImTextureID thumbnailTextureId);
};