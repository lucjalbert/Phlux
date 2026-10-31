#pragma once

#include <glm/glm.hpp>

namespace Utils {

	struct Ray
	{
		glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
	};

	// Uses Moller Trumbore double sided alg
	bool RayTriangleIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, float& outHitDistance);

	Ray GenerateRayFromMousePos(float mousePosX, float mousePosY, float windowWidth, float windowHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
}