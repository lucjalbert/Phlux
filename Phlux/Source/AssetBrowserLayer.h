#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"

#include <glm/glm.hpp>

#include <imgui.h>

#include <cstdint>

class AssetBrowserLayer : public Core::Layer
{
public:
	AssetBrowserLayer();

	void OnImGuiRender() override;

private:
	ImVec2 m_TileSize = ImVec2(100.0f, 120.0f);

	uint32_t m_FrameBufferObject = 0;
	glm::vec2 m_ThumbnailResolution = glm::vec2(256.0f, 256.0f);

	void DrawAssetBrowserTile(const char* label, ImVec2 tileSize, int id);
};