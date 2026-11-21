#pragma once

#include "Core/Layer.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/AssetRegistry/AssetRegistry.h"

#include "GLFW/glfw3.h"

#include <imgui.h>
#include <imfilebrowser.h>

#include <string>

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
	GLFWwindow* m_Window;
	AssetRegistry::AssetRegistry& m_AssetRegistry;

	bool m_ShowDemoWindow = false;

	float m_RightSideButtonsWidth = 35.0f;
	std::string m_CloseButtonLabel = std::string("X");
	std::string m_MaximizeButtonLabel = std::string("[]");
	std::string m_MinimizeButtonLabel = std::string("_");

	bool m_IsWindowBeingDragged = false;
};