#include "ViewportLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Input/Input.h"
#include "Core/Camera/FlyCamera.h"
#include "Core/Scene/Actors/Actor.h"
#include "Core/Scene/Actors/ModelActor.h"

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

	m_InputManager = Core::Application::Get().GetInputManager().get();
	m_Scene = Core::Application::Get().GetScene().get();
	m_Camera = m_Scene->GetCamera().get();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	m_Shader.Use();

	// Temp auto-import model
	Renderer::Model model = Renderer::Model("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\backpack.obj");

	Actor::ModelActor modelActor = Actor::ModelActor(model);
	Core::Application::Get().GetScene()->AddActor<Actor::ModelActor>(std::move(model));
}

ViewportLayer::~ViewportLayer()
{
	m_Shader.Destroy();

	std::println("Destroying Viewport Layer");
}

void ViewportLayer::OnUpdate(float deltaTime)
{
	if (m_InputManager->IsKeyPressed(GLFW_KEY_SPACE))
	{
		glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_FILL : GL_LINE);
		m_WireframeMode = !m_WireframeMode;
	}

	if (m_InputManager->IsKeyPressed(GLFW_KEY_TAB))
	{
		m_WireOnShadedMode = !m_WireOnShadedMode;
	}

	// Handle camera cursor permissions
	if (m_InputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = true;
		glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (m_InputManager->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = false;
		glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (m_InputManager->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
	{
		float mousePosX = Core::Application::Get().GetInputManager()->GetMousePosX();
		float mousePosY = Core::Application::Get().GetInputManager()->GetMousePosY();

		m_Scene->SelectActorAtMouseLocation(mousePosX, mousePosY);
	}
	
	if (m_CameraMode)
	{
		// Camera direction
		m_Camera->UpdateCameraRotation(m_InputManager->GetMouseDeltaPosX(), m_InputManager->GetMouseDeltaPosY());

		// Camera position
		if (m_InputManager->IsKeyDown(GLFW_KEY_W))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::FORWARD, deltaTime);

		if (m_InputManager->IsKeyDown(GLFW_KEY_S))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::BACKWARD, deltaTime);

		if (m_InputManager->IsKeyDown(GLFW_KEY_D))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::RIGHT, deltaTime);

		if (m_InputManager->IsKeyDown(GLFW_KEY_A))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::LEFT, deltaTime);

		if (m_InputManager->IsKeyDown(GLFW_KEY_E))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::UP, deltaTime);

		if (m_InputManager->IsKeyDown(GLFW_KEY_Q))
			m_Camera->UpdateCameraPosition(Camera::CameraDirection::DOWN, deltaTime);
	}
}

void ViewportLayer::OnRender()
{
	glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	view = m_Camera->GetViewMatrix();

	glm::mat4 projection;
	projection = m_Camera->GetProjectionMatrix();

	m_Shader.Use();
	m_Shader.SetMat4("view", view);
	m_Shader.SetMat4("projection", projection);

	for (uint32_t i = 0; i < m_Scene->GetNumActors(); i++)
	{
		auto& actor = m_Scene->GetActors()[i];
		m_Shader.SetMat4("model", m_Scene->GetActors()[i]->GetModelMatrix());

		m_Shader.Use();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		m_Shader.SetBool("bIsSelected", false);
		m_Shader.SetBool("bIsWireframe", false);
		actor->Render(); // Fill

		if (m_WireOnShadedMode || actor->bSelected)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_Shader.SetBool("bIsWireframe", true);
			m_Shader.SetBool("bIsSelected", actor->bSelected);
			actor->Render(); // Wireframe
		}
	}
}