#include "Application.h"

#include "Window.h"
#include "Layer.h"
#include "Input/Input.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assert.h>
#include <iostream>

namespace Core {

	static Application* s_Application = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "[GLFW Error]" << description << std::endl;
	}

	Application::Application(ApplicationSpecification specification)
		: m_Specification(specification)
	{
		s_Application = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		if (m_Specification.WindowSpec.Title.empty())
		{
			m_Specification.WindowSpec.Title = m_Specification.Name;
		}

		m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		m_Scene = std::make_shared<Scene::Scene>(Scene::Scene());

		m_Input = std::make_shared<Input::Input>();
		m_Input->Init(m_Window->GetHandle());

		m_ImGuiLayer = std::make_unique<UI::ImGuiLayer>();
		m_ImGuiLayer->Initialize(m_Window->GetHandle());
	}

	Application::~Application()
	{
		m_Window->Destroy();

		m_ImGuiLayer->Shutdown();

		glfwTerminate();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;
		float lastFrameTime = 0.0f;

		while (m_Running)
		{
			// Reset inputs that were pressed or released last frame before polling events
			m_Input->BeginFrame();

			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			float currentFrameTime = GetTime();
			float deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnRender();

			m_ImGuiLayer->BeginFrame();

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->EndFrame();

			m_Window->Update();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	glm::vec2 Application::GetFramebufferSize() const
	{
		return m_Window->GetFramebufferSize();
	}

	float Application::GetTime()
	{
		return glfwGetTime();
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}
}