#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Camera/FlyCamera.h"

#include <glm/glm.hpp>

#include <stdint.h>

class ViewportLayer : public Core::Layer
{
public:
	ViewportLayer();
	virtual ~ViewportLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;
private:
	Renderer::Shader m_Shader;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;
	uint32_t m_ElementBuffer = 0;

	bool m_WireframeMode = false;
	bool m_PerspectiveMode = true;

	bool m_CameraMode = false;
	Camera::FlyCamera m_Camera;
};