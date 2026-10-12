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

	// Quad made out of 2 triangles
	float vertices[] = {
		// Position			// Color			// Tex Coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);

	glGenBuffers(1, &m_ElementBuffer);

	// Store data inside created buffers
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define the way our data is stored
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0); // Position

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // Color

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); // Tex Coords

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
}

void ViewportLayer::OnRender()
{
	m_Shader.Use();

	glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_Shader.SetFloat("time", Core::Application::Get().GetTime());
	m_Shader.SetTexture("ourTexture", 0, GL_TEXTURE0);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)Core::Application::Get().GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	m_Shader.SetMat4("transform", trans);

	glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}