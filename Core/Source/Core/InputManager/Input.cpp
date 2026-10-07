#include "Input.h"

#include <algorithm>

void Input::Init(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyCallback);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
	input->OnKey(key, action);
}

void Input::BeginFrame()
{
	std::fill(
		std::begin(m_KeysPressed), 
		std::end(m_KeysReleased), 
		false
	);
}

void Input::OnKey(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		m_KeysDown[key] = true;
		m_KeysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		m_KeysDown[key] = false;
		m_KeysReleased[key] = true;
	}

}

bool Input::IsKeyDown(int key)
{
	return m_KeysDown[key];
}

bool Input::IsKeyPressed(int key)
{
	return m_KeysPressed[key];
}

bool Input::IsKeyReleased(int key)
{
	return m_KeysReleased[key];
}