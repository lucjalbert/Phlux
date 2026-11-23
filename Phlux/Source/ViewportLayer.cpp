#include "ViewportLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/AssetRegistry/ModelLibrary.h"
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
	m_MaterialLibrary(Core::Application::Get().GetAssetRegistry().GetMaterialLibrary()),
	m_ShaderLibrary(Core::Application::Get().GetAssetRegistry().GetShaderLibrary()),
	m_ViewportBufferSize(Core::Application::Get().GetViewportBufferSize())
{
	std::println("Initializing Viewport Layer");	

	SetupViewportBuffer();

	// Temp auto-import test model on startup
	AssetRegistry::ModelId modelId = AssetRegistry::ModelId::Default();
	m_Scene.AddActor<Actor::ModelActor>(modelId);
}

ViewportLayer::~ViewportLayer()
{
	std::println("Destroying Viewport Layer");
}

void ViewportLayer::SetupViewportBuffer()
{
	glGenFramebuffers(1, &m_ViewportFBO);
	glGenRenderbuffers(1, &m_ViewportDepthRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ViewportFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ViewportDepthRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_ViewportBufferSize.x), static_cast<GLsizei>(m_ViewportBufferSize.y));
	glViewport(0, 0, static_cast<GLsizei>(m_ViewportBufferSize.x), static_cast<GLsizei>(m_ViewportBufferSize.y));

	Renderer::Texture2DSpecification viewportTextureSettings = Renderer::Texture2DSpecification();
	viewportTextureSettings.Width = static_cast<uint32_t>(m_ViewportBufferSize.x);
	viewportTextureSettings.Height = static_cast<uint32_t>(m_ViewportBufferSize.y);
	m_ViewportBuffer = std::make_unique<Renderer::Texture2D>(viewportTextureSettings);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ViewportBuffer->GetID(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ViewportDepthRBO);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
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

	// Keep viewport buffer size in sync with ImGui window size
	m_ViewportBufferSize = Core::Application::Get().GetViewportBufferSize();
}

void ViewportLayer::OnRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ViewportFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ViewportDepthRBO);

	glm::vec2 viewportbufferSize = Core::Application::Get().GetViewportBufferSize();
	glViewport(0, 0, static_cast<GLsizei>(viewportbufferSize.x), static_cast<GLsizei>(viewportbufferSize.y));

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

		const Renderer::Material& material = m_MaterialLibrary.Get(modelActor->GetMaterialId());
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

		shader.SetUniform("bIsWireframe", false);
		shader.SetUniform("bIsSelected", false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void ViewportLayer::OnImGuiRender()
{
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	Core::Application::Get().SetViewportBufferSize(glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));

	ImTextureID textureId = m_ViewportBuffer->GetID();
	ImGui::Image(textureId, ImVec2(m_ViewportBufferSize.x, m_ViewportBufferSize.y), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
		m_InputManager.SetIsViewportFocused(true);
	else
		m_InputManager.SetIsViewportFocused(false);

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

	ImGui::End();
}