#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/AssetRegistry/MaterialLibrary.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Scene/Scene.h"
#include "Core/Input/Input.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <imfilebrowser.h>
#include <ImGuizmo.h>

#include <stdint.h>
#include <vector>
#include <memory>

class ViewportLayer : public Core::Layer
{
public:
	ViewportLayer();
	virtual ~ViewportLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

	void SetupViewportBuffer();

private:
	bool m_WireframeMode = false;
	bool m_WireOnShadedMode = true;
	bool m_CameraMode = false;

	Input::Input& m_InputManager;
	Scene::Scene& m_Scene;
	Camera::FlyCamera& m_Camera;
	const AssetRegistry::MaterialLibrary& m_MaterialLibrary;
	const AssetRegistry::ShaderLibrary& m_ShaderLibrary;

	bool m_Enable3DGuizmo = false;
	ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

	GLuint m_ViewportFBO = 0;
	GLuint m_ViewportDepthRBO = 0;
	std::unique_ptr<Renderer::Texture2D> m_ViewportBuffer;
	glm::vec2 m_ViewportBufferSize;
};