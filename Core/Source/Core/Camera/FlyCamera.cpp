#include "FlyCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Camera {

	FlyCamera::FlyCamera(FlyCameraSpecification flyCameraSpec)
		: m_FlyCameraSpec(flyCameraSpec)
	{
	}

	glm::mat4 FlyCamera::GetViewMatrix()
	{
		return glm::lookAt(m_FlyCameraSpec.CameraPos,m_FlyCameraSpec.CameraPos + m_FlyCameraSpec.CameraFront, m_FlyCameraSpec.CameraUp);
	}

	void FlyCamera::UpdateCameraPosition(CameraDirection direction, float deltaTime)
	{
		float cameraSpeed = m_FlyCameraSpec.BaseCameraSpeed * deltaTime;
		switch (direction)
		{
		case CameraDirection::FORWARD:
			m_FlyCameraSpec.CameraPos += cameraSpeed * m_FlyCameraSpec.CameraFront;
			break;
			
		case CameraDirection::BACKWARD:
			m_FlyCameraSpec.CameraPos -= cameraSpeed * m_FlyCameraSpec.CameraFront;
			break;

		case CameraDirection::RIGHT:
			m_FlyCameraSpec.CameraPos += cameraSpeed * glm::normalize(glm::cross(m_FlyCameraSpec.CameraFront, m_FlyCameraSpec.CameraUp));
			break;

		case CameraDirection::LEFT:
			m_FlyCameraSpec.CameraPos -= cameraSpeed * glm::normalize(glm::cross(m_FlyCameraSpec.CameraFront, m_FlyCameraSpec.CameraUp));
			break;

		case CameraDirection::UP:
			m_FlyCameraSpec.CameraPos += cameraSpeed * m_FlyCameraSpec.CameraUp;
			break;

		case CameraDirection::DOWN:
			m_FlyCameraSpec.CameraPos -= cameraSpeed * m_FlyCameraSpec.CameraUp;
			break;

		default:
			return;
		}	
	}

	void FlyCamera::UpdateCameraRotation(float deltaPosX, float deltaPosY, bool constrainPitch)
	{
		m_CameraYaw += deltaPosX * m_FlyCameraSpec.CameraSensitivity;
		m_CameraPitch += deltaPosY * m_FlyCameraSpec.CameraSensitivity * -1.0f;

		if (constrainPitch)
		{
			if (m_CameraPitch > 89.0f)
				m_CameraPitch = 89.0f;
			if (m_CameraPitch < -89.0f)
				m_CameraPitch = -89.0f;
		}
		
		m_CameraDirection.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraDirection.y = sin(glm::radians(m_CameraPitch));
		m_CameraDirection.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_FlyCameraSpec.CameraFront = glm::normalize(m_CameraDirection);
	}
}