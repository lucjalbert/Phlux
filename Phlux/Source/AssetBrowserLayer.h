#pragma once

#include "Core/Layer.h"

#include <imgui.h>

class AssetBrowserLayer : public Core::Layer
{
public:
	AssetBrowserLayer();

	void OnImGuiRender() override;
};