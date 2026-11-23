#pragma once

#include "Window.h"
#include "Layer.h"
#include "Input/Input.h"
#include "ImGui/ImGuiLayer.h"
#include "Scene/Scene.h"
#include "Renderer/AssetRegistry/AssetRegistry.h"

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

		Window& GetWindow() const { return *m_Window; }
		Input::Input& GetInputManager() const { return *m_Input; }
		Scene::Scene& GetScene() const { return *m_Scene; }
		AssetRegistry::AssetRegistry& GetAssetRegistry() const { return *m_AssetRegistry; }

		static Application& Get();
		static float GetTime();

		glm::vec2 GetFramebufferSize() const;

		glm::vec2 GetViewportBufferSize() const;
		void SetViewportBufferSize(glm::vec2 viewportBufferSize);
	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;

		glm::vec2 m_ViewportBufferSize = glm::vec2(1280.0f, 720.0f);

		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		std::unique_ptr<UI::ImGuiLayer> m_ImGuiLayer;

		std::unique_ptr<Input::Input> m_Input;

		std::unique_ptr<Scene::Scene> m_Scene;

		std::unique_ptr<AssetRegistry::AssetRegistry> m_AssetRegistry;
	};
}