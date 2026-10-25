#pragma once

#include <glm/glm.hpp>

namespace Camera {

	enum class CameraDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	struct FlyCameraSpecification
	{
		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float BaseCameraSpeed = 4.0f;
		float CameraSensitivity = 0.1f;
	};

	class FlyCamera
	{
	public:
		FlyCamera(FlyCameraSpecification flyCameraSpec = FlyCameraSpecification());

		glm::mat4 GetViewMatrix();

		void UpdateCameraPosition(CameraDirection direction, float deltaTime);
		void UpdateCameraRotation(float deltaPosX, float deltaPosY, bool constrainPitch = true);

	private:
		FlyCameraSpecification m_FlyCameraSpec;

		glm::vec3 m_CameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		float m_CameraYaw = -90.0f;
		float m_CameraPitch = 0.0f;
	};
}