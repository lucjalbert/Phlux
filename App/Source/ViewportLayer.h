#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <stdint.h>

class ViewportLayer : public Core::Layer
{
public:
	ViewportLayer();
	virtual ~ViewportLayer();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
private:
	Renderer::Shader m_Shader;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;
	uint32_t m_ElementBuffer = 0;

	bool m_WireframeMode = false;
	bool m_PerspectiveMode = true;

	bool m_CameraMode = false;

	glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_BaseCameraSpeed = 4.0f;
	float m_CameraSensitivity = 0.1f;

	glm::vec3 m_CameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_CameraYaw = -90.0f;
	float m_CameraPitch = 0.0f;
};