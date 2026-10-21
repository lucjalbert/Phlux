#pragma once

#include <GLFW/glfw3.h>

#include "Core/Layer.h"

namespace UI {

	class ImGuiLayer : public Core::Layer
	{
	public:
		void Initialize(GLFWwindow* window);
		void Shutdown();

		void BeginFrame();
		void EndFrame();
	};
}