#include "Window.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <iostream>
#include <assert.h>
#include <print>

namespace Core {

	Window::Window(const WindowSpecification& specification)
		: m_Specification(specification)
	{
	}

	Window::~Window()
	{
		Destroy();
	}

	void Window::Create()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Handle = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str(), nullptr, nullptr);

		if (!m_Handle)
		{
			std::cerr << "Failed to create GLFW window!\n";
			assert(false);
		}

		glfwMakeContextCurrent(m_Handle);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	void Window::Destroy()
	{
		if (m_Handle)
		{
			glfwDestroyWindow(m_Handle);
			m_Handle = nullptr;
		}
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_Handle);
	}

	glm::vec2 Window::GetFramebufferSize() const
	{
		if (m_Handle)
		{
			int width, height;
			glfwGetFramebufferSize(m_Handle, &width, &height);
			return { width, height };
		}
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Handle) != 0;
	}
}