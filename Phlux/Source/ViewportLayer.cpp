#include "ViewportLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Input/Input.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Scene/Actors/Actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <print>
#include <iostream>
#include <stdint.h>
#include <vector>

#include <imgui.h>
#include <imfilebrowser.h>

ViewportLayer::ViewportLayer()
	: m_Shader(Renderer::Shader("Shaders/default.vert", "Shaders/default.frag"))
{
	std::println("Initializing Viewport Layer");

	// Initialize camera
	Camera::FlyCameraSpecification flyCameraSpec;
	m_Camera = Camera::FlyCamera(flyCameraSpec);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	m_Shader.Use();
}

ViewportLayer::~ViewportLayer()
{
	m_Shader.Destroy();

	std::println("Destroying Viewport Layer");
}

void ViewportLayer::OnUpdate(float deltaTime)
{
	if (Core::Application::Get().GetInputManager()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		glPolygonMode(
			GL_FRONT_AND_BACK, 
			m_WireframeMode ? GL_FILL : GL_LINE
		);
		m_WireframeMode = !m_WireframeMode;
	}

	if (Core::Application::Get().GetInputManager()->IsKeyPressed(GLFW_KEY_TAB))
	{
		m_PerspectiveMode = !m_PerspectiveMode;
	}

	// Handle camera cursor permissions
	if (Core::Application::Get().GetInputManager()->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = true;
		glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (Core::Application::Get().GetInputManager()->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = false;
		glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (m_CameraMode)
	{
		// Camera direction
		m_Camera.UpdateCameraRotation(Core::Application::Get().GetInputManager()->GetMouseDeltaPosX(), Core::Application::Get().GetInputManager()->GetMouseDeltaPosY());

		// Camera position
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_W))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::FORWARD, deltaTime);
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_S))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::BACKWARD, deltaTime);
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_D))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::RIGHT, deltaTime);
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_A))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::LEFT, deltaTime);
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_E))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::UP, deltaTime);
		if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_Q))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::DOWN, deltaTime);
	}
}

void ViewportLayer::OnRender()
{
	glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	view = m_Camera.GetViewMatrix();

	glm::mat4 projection;
	if (m_PerspectiveMode)
	{
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
	}
	else
	{
		projection = glm::ortho(0.0f, 6.0f, 0.0f, 5.0f, -1000.0f, 10000.0f);
	}

	m_Shader.SetMat4("view", view);
	m_Shader.SetMat4("projection", projection);

	Scene::Scene* scene = Core::Application::Get().GetScene().get();
	for (uint32_t i = 0; i < scene->GetNumActors(); i++)
	{
		auto& actor = scene->GetActors()[i];
		m_Shader.SetMat4("model", scene->GetActors()[i]->GetModelMatrix());

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		m_Shader.SetBool("bIsSelected", false);
		m_Shader.SetBool("bIsWireframe", false);
		actor->Render(); // Fill

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_Shader.SetBool("bIsWireframe", true);
		m_Shader.SetBool("bIsSelected", actor->bSelected);
		actor->Render(); // Wireframe
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}