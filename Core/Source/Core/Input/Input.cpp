#include "Input.h"

#include <print>
#include <algorithm>

namespace Input {

	void Input::Init(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
	}

	void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
		input->OnKey(key, action);
	}

	void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
		input->OnMouseButton(button, action);
	}

	void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
		input->OnCursorMoved(xpos, ypos);
	}

	void Input::BeginFrame()
	{
		std::fill(
			std::begin(m_KeysPressed),
			std::end(m_KeysReleased),
			false
		);

		m_ButtonsPressed.clear();
		m_ButtonsReleased.clear();

		m_MouseDeltaPosX = 0.0f;
		m_MouseDeltaPosY = 0.0f;
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

	void Input::OnMouseButton(int button, int action)
	{
		if (action == GLFW_PRESS)
		{
			m_ButtonsDown.insert(button);
			m_ButtonsPressed.insert(button);
		}
		else if (action == GLFW_RELEASE)
		{
			m_ButtonsDown.erase(button);
			m_ButtonsReleased.insert(button);
		}
	}

	bool Input::IsMouseButtonDown(int button)
	{
		return m_ButtonsDown.contains(button);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		return m_ButtonsPressed.contains(button);
	}

	bool Input::IsMouseButtonReleased(int button)
	{
		return m_ButtonsReleased.contains(button);
	}

	void Input::OnCursorMoved(double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		m_MouseDeltaPosX = xpos - m_LastCameraPosX;
		m_MouseDeltaPosY = ypos - m_LastCameraPosY;
		m_LastCameraPosX = xpos;
		m_LastCameraPosY = ypos;
	}
}