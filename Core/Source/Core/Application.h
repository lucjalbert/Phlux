#pragma once

#include "Window.h"
#include "Layer.h"
#include "Input/Input.h"
#include "ImGui/ImGuiLayer.h"
#include "Scene/Scene.h"

#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace Core {

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(ApplicationSpecification WindowSpec = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}

		std::shared_ptr<Window> GetWindow() const { return m_Window; }
		std::shared_ptr<Input::Input> GetInputManager() const { return m_Input; }
		std::shared_ptr<Scene::Scene> GetScene() const { return m_Scene; }

		static Application& Get();
		static float GetTime();

		glm::vec2 GetFramebufferSize() const;
	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;

		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		std::unique_ptr<UI::ImGuiLayer> m_ImGuiLayer;

		std::shared_ptr<Input::Input> m_Input;

		std::shared_ptr<Scene::Scene> m_Scene;
	};
}