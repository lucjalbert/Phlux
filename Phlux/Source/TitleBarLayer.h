#pragma once

#include "Core/Layer.h"
#include "Core/Scene/Scene.h"

#include <imgui.h>
#include <imfilebrowser.h>

class TitleBarLayer : public Core::Layer
{
public:
	TitleBarLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnImGuiRender() override;

private:
	ImGui::FileBrowser m_ImportObjDialog;
	ImGui::FileBrowser m_SaveSceneDialog;
	ImGui::FileBrowser m_LoadSceneDialog;

	Scene::Scene& m_Scene;

	bool m_ShowDemoWindow = false;
};