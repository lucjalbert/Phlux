#include "RayCast.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utils {

	bool RayTriangleIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, float& outHitDistance)
	{
		const float EPSILON = 0.0000001f;

		glm::vec3 edge1 = vert1 - vert0;
		glm::vec3 edge2 = vert2 - vert0;

		glm::vec3 cross_RayDir_Edge2 = glm::cross(direction, edge2);

		float determinant = glm::dot(edge1, cross_RayDir_Edge2);
		if (determinant > EPSILON && determinant < EPSILON)
			return false;

		float inv_determinant = 1.0f / determinant;

		glm::vec3 origin_Minus_Vert0 = origin - vert0;

		float baryU = glm::dot(origin_Minus_Vert0, cross_RayDir_Edge2) * inv_determinant;
		if (baryU < 0.0f || baryU > 1.0f)
			return false;

		glm::vec3 cross_OriginMinusVert0_Edge1 = glm::cross(origin_Minus_Vert0, edge1);

		float baryV = glm::dot(direction, cross_OriginMinusVert0_Edge1) * inv_determinant;
		if (baryV < 0.0f || baryU + baryV > 1.0f)
			return false;

		outHitDistance = glm::dot(edge2, cross_OriginMinusVert0_Edge1) * inv_determinant;

		return true;
	}

	Ray GenerateRayFromMousePos(float mousePosX, float mousePosY, float windowWidth, float windowHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		float flippedMousePosY = static_cast<float>(windowHeight) - mousePosY;

		glm::vec3 screenPosNear = glm::vec3(mousePosX, flippedMousePosY, 0.0f);
		glm::vec3 screenPosFar = glm::vec3(mousePosX, flippedMousePosY, 1.0f);

		glm::vec4 viewportBoundingBox = glm::vec4(0.0f, 0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight));

		glm::vec3 worldPosNear = glm::unProject(screenPosNear, viewMatrix, projectionMatrix, viewportBoundingBox);
		glm::vec3 worldPosFar = glm::unProject(screenPosFar, viewMatrix, projectionMatrix, viewportBoundingBox);

		Ray mouseRay;

		mouseRay.origin = worldPosNear;
		mouseRay.direction = glm::normalize(worldPosFar - worldPosNear);

		return mouseRay;
	}
}