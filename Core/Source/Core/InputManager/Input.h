#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
	void Init(GLFWwindow* window);

	void BeginFrame();

	bool IsKeyDown(int key);
	bool IsKeyPressed(int key);
	bool IsKeyReleased(int key);

private:
	void OnKey(int key, int action);

	bool m_KeysDown[GLFW_KEY_LAST + 1];
	bool m_KeysPressed[GLFW_KEY_LAST + 1];
	bool m_KeysReleased[GLFW_KEY_LAST + 1];

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};