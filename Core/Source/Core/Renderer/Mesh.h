#pragma once

#include <glm/glm.hpp>

#include <json.hpp>

#include <vector>
namespace Renderer {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	// Serialize Vertex
	void to_json(nlohmann::json& j, const Vertex& v);
	void from_json(const nlohmann::json& j, Vertex& v);

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		~Mesh();

		// Disallow copying meshes to avoid double deleting buffers
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		
		void Draw();

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		uint32_t m_VertexArray = 0;
		uint32_t m_VertexBuffer = 0;
		uint32_t m_ElementBuffer = 0;

		void SetupMesh();
	};
}