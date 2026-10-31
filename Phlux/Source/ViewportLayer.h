#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Model.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Scene/Scene.h"
#include "Core/Input/Input.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <imfilebrowser.h>

#include <stdint.h>
#include <vector>

class ViewportLayer : public Core::Layer
{
public:
	ViewportLayer();
	virtual ~ViewportLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
private:
	Renderer::Shader m_Shader;

	bool m_WireframeMode = false;
	bool m_WireOnShadedMode = true;
	bool m_CameraMode = false;

	Input::Input* m_InputManager;
	Scene::Scene* m_Scene;
	Camera::FlyCamera* m_Camera;
};