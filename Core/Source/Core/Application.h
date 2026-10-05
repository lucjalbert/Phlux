#pragma once

#include "Window.h"

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

		Application& Get();

		glm::vec2 GetFrameBufferSize() const;
	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;
	};
}