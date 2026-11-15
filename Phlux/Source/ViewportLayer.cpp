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
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <imgui.h>
#include <imfilebrowser.h>
#include <ImGuizmo.h>

#include <print>
#include <iostream>
#include <stdint.h>
#include <vector>

ViewportLayer::ViewportLayer()
	: m_InputManager(Core::Application::Get().GetInputManager()),
	m_Scene(Core::Application::Get().GetScene()),
	m_Camera(m_Scene.GetCamera()),
	m_MaterialLibrary(m_Scene.GetMaterialLibrary()),
	m_ShaderLibrary(Core::Application::Get().GetShaderLibrary())
{
	std::println("Initializing Viewport Layer");	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	// Temp auto-import test model on startup
	Renderer::Model model = Renderer::Model("D:\\_LUC\\__MYPROJECTS\\Phlux\\Content\\Meshes\\Cube.obj");
	m_Scene.AddActor<Actor::ModelActor>(std::move(model));
}

ViewportLayer::~ViewportLayer()
{
	std::println("Destroying Viewport Layer");
}

void ViewportLayer::OnUpdate(float deltaTime)
{
	if (m_InputManager.IsKeyPressed(GLFW_KEY_SPACE))
	{
		glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_FILL : GL_LINE);
		m_WireframeMode = !m_WireframeMode;
	}

	if (m_InputManager.IsKeyPressed(GLFW_KEY_TAB))
	{
		m_WireOnShadedMode = !m_WireOnShadedMode;
	}

	// Handle camera cursor permissions
	if (m_InputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = true;
		glfwSetInputMode(Core::Application::Get().GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (m_InputManager.IsMouseButtonReleased(GLFW_MOUSE_BUTTON_2))
	{
		m_CameraMode = false;
		glfwSetInputMode(Core::Application::Get().GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (m_InputManager.IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
	{
		float mousePosX = Core::Application::Get().GetInputManager().GetMousePosX();
		float mousePosY = Core::Application::Get().GetInputManager().GetMousePosY();

		m_Scene.SelectActorAtMouseLocation(mousePosX, mousePosY);
	}
	
	if (m_CameraMode)
	{
		// Camera direction
		m_Camera.UpdateCameraRotation(m_InputManager.GetMouseDeltaPosX(), m_InputManager.GetMouseDeltaPosY());

		// Camera position
		if (m_InputManager.IsKeyDown(GLFW_KEY_W))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::FORWARD, deltaTime);

		if (m_InputManager.IsKeyDown(GLFW_KEY_S))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::BACKWARD, deltaTime);

		if (m_InputManager.IsKeyDown(GLFW_KEY_D))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::RIGHT, deltaTime);

		if (m_InputManager.IsKeyDown(GLFW_KEY_A))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::LEFT, deltaTime);

		if (m_InputManager.IsKeyDown(GLFW_KEY_E))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::UP, deltaTime);

		if (m_InputManager.IsKeyDown(GLFW_KEY_Q))
			m_Camera.UpdateCameraPosition(Camera::CameraDirection::DOWN, deltaTime);
	}

	if (!m_CameraMode)
	{
		// 3D Guizmo
		if (m_InputManager.IsKeyPressed(GLFW_KEY_Q))
		{
			m_Enable3DGuizmo = false;
		}
		if (m_InputManager.IsKeyPressed(GLFW_KEY_W))
		{
			m_Enable3DGuizmo = true;
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (m_InputManager.IsKeyPressed(GLFW_KEY_E))
		{
			m_Enable3DGuizmo = true;
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}
		if (m_InputManager.IsKeyPressed(GLFW_KEY_R))
		{
			m_Enable3DGuizmo = true;
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;
		}

		if (m_InputManager.IsKeyPressed(GLFW_KEY_DELETE))
		{
			m_Scene.DeleteSelectedActor();
		}
	}
}

void ViewportLayer::OnRender()
{
	glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	glClearColor(0.18f, 0.20f, 0.27f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	view = m_Camera.GetViewMatrix();

	glm::mat4 projection;
	projection = m_Camera.GetProjectionMatrix();

	for (const auto& actor : m_Scene.GetActors())
	{
		auto modelActor = dynamic_cast<Actor::ModelActor*>(actor.get());
		if (!modelActor)
			continue;

		const Renderer::Material& material = m_MaterialLibrary.Get(modelActor->GetMaterial());
		const Renderer::Shader& shader = m_ShaderLibrary.Get(material.GetShaderId());
		shader.Use();
		material.ApplyParams(shader);

		shader.SetUniform("view", view);
		shader.SetUniform("projection", projection);
		shader.SetUniform("model", modelActor->GetModelMatrix());

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		shader.SetUniform("bIsSelected", false);
		shader.SetUniform("bIsWireframe", false);
		actor->Render(); // Fill

		if (m_WireOnShadedMode || actor->bSelected)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			shader.SetUniform("bIsWireframe", true);
			shader.SetUniform("bIsSelected", actor->bSelected);
			actor->Render(); // Wireframe
		}
	}
}

void ViewportLayer::OnImGuiRender()
{
	if (m_Enable3DGuizmo)
	{
		ImGuiIO io = ImGui::GetIO();
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		if (const auto& selectedActor = m_Scene.GetSelectedActor())
		{
			glm::mat4& modelMatrix = selectedActor->GetModelMatrix();
			ImGuizmo::Manipulate(glm::value_ptr(m_Camera.GetViewMatrix()), glm::value_ptr(m_Camera.GetProjectionMatrix()), m_GuizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(modelMatrix));
		}
	}
}