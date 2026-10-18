#pragma once

#include <GLFW/glfw3.h>

#include <unordered_set>

namespace Input {
	
	class Input
	{
	public:
		void Init(GLFWwindow* window);

		void BeginFrame();

		bool IsKeyDown(int key);
		bool IsKeyPressed(int key);
		bool IsKeyReleased(int key);

		bool IsMouseButtonDown(int button);
		bool IsMouseButtonPressed(int button);
		bool IsMouseButtonReleased(int button);

		float GetMouseDeltaPosX() const { return m_MouseDeltaPosX; }
		float GetMouseDeltaPosY() const { return m_MouseDeltaPosY; }

	private:
		void OnKey(int key, int action);

		bool m_KeysDown[GLFW_KEY_LAST + 1];
		bool m_KeysPressed[GLFW_KEY_LAST + 1];
		bool m_KeysReleased[GLFW_KEY_LAST + 1];

		void OnMouseButton(int button, int action);

		std::unordered_set<int> m_ButtonsDown;
		std::unordered_set<int> m_ButtonsPressed;
		std::unordered_set<int> m_ButtonsReleased;

		void OnCursorMoved(double xpos, double ypos);

		float m_LastCameraPosX = 0.0f;
		float m_LastCameraPosY = 0.0f;

		float m_MouseDeltaPosX = 0.0f;
		float m_MouseDeltaPosY = 0.0f;

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double xposIn, double yposIn);
	};
}