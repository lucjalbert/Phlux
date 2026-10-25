#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace Renderer {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		
		void Draw();

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		uint32_t m_VertexArray = 0;
		uint32_t m_VertexBuffer = 0;
		uint32_t m_ElementBuffer = 0;

		void SetupMesh();
	};
}