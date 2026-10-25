#pragma once

#include "Core/Layer.h"

#include <imgui.h>
#include <imfilebrowser.h>

class TitleBarLayer : public Core::Layer
{
public:
	TitleBarLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnImGuiRender() override;

private:
	ImGui::FileBrowser m_FileDialog;

	bool m_ShowDemoWindow = false;
};