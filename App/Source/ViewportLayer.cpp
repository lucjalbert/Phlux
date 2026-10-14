#include "ViewportLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Input/Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <print>
#include <iostream>
#include <stdint.h>

ViewportLayer::ViewportLayer()
	: m_Shader(Renderer::Shader("Shaders/basicTexture.vert", "Shaders/basicTexture.frag"))
{
	std::println("Initializing Viewport Layer");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);

	glGenBuffers(1, &m_ElementBuffer);

	// Store data inside created buffers
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Define the way our data is stored
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0); // Position

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); // Tex Coords

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	// Create texture
	Renderer::Texture2DSpecification Texture2DSpec;
	Texture2DSpec.path = "Textures/Container.jpg";
	std::shared_ptr<Renderer::Texture2D> texture = std::make_shared<Renderer::Texture2D>(Texture2DSpec);
	texture->Load();

	m_Shader.AddTexture(texture);
}

ViewportLayer::~ViewportLayer()
{
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_ElementBuffer);
	glDeleteBuffers(1, &m_VertexArray);

	m_Shader.Destroy();

	std::println("Destroying Viewport Layer");
}

void ViewportLayer::OnUpdate(float ts)
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

	if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_UP))
	{
		m_CameraPos += glm::vec3(0.0f, m_CameraSpeed * ts, 0.0f);
	}
	if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_DOWN))
	{
		m_CameraPos += glm::vec3(0.0f, -m_CameraSpeed * ts, 0.0f);
	}
	if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_RIGHT))
	{
		m_CameraPos += glm::vec3(m_CameraSpeed * ts, 0.0f, 0.0f);
	}
	if (Core::Application::Get().GetInputManager()->IsKeyDown(GLFW_KEY_LEFT))
	{
		m_CameraPos += glm::vec3(m_CameraSpeed * ts, 0.0f, 0.0f);
	}

	if (Core::Application::Get().GetInputManager()->IsKeyPressed(GLFW_KEY_F))
	{
		m_CameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void ViewportLayer::OnRender()
{
	m_Shader.Use();

	glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader.SetFloat("time", Core::Application::Get().GetTime());
	m_Shader.SetTexture("ourTexture", 0, GL_TEXTURE0);

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, 4.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, 4.8f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, 3.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, 2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	const float radius = 10.0f;
	float camX = sin(Core::Application::Get().GetTime()) * radius;
	float camZ = cos(Core::Application::Get().GetTime()) * radius;

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
		glm::vec3(m_CameraPos),
		glm::vec3(0.0f, 1.0f, 0.0f));

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

	glBindVertexArray(m_VertexArray);

	for (uint32_t i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.f * (i + 1);
		model = glm::rotate(model, (float)Core::Application::Get().GetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader.SetMat4("model", model);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}