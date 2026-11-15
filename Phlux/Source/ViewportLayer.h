#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/MaterialLibrary.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Scene/Scene.h"
#include "Core/Input/Input.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <imfilebrowser.h>
#include <ImGuizmo.h>

#include <stdint.h>
#include <vector>

class ViewportLayer : public Core::Layer
{
public:
	ViewportLayer();
	virtual ~ViewportLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

private:
	bool m_WireframeMode = false;
	bool m_WireOnShadedMode = true;
	bool m_CameraMode = false;

	Input::Input& m_InputManager;
	Scene::Scene& m_Scene;
	Camera::FlyCamera& m_Camera;
	const Renderer::MaterialLibrary& m_MaterialLibrary;
	const Renderer::ShaderLibrary& m_ShaderLibrary;

	bool m_Enable3DGuizmo = false;
	ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
};