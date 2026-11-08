#include "Mesh.h"

#include <glad/glad.h>

#include <json.hpp>

#include <iostream>

namespace Renderer {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
		: m_Vertices(std::move(vertices)), m_Indices(std::move(indices))
	{
		SetupMesh();
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_ElementBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);
	}

	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &m_VertexArray);
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_ElementBuffer);

		glBindVertexArray(m_VertexArray);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void Mesh::Draw()
	{
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void to_json(nlohmann::json& j, const Vertex& v)
	{
		j = nlohmann::json{ {"positionX", v.Position.x}, {"positionY", v.Position.y}, {"positionZ", v.Position.z},
							{"normalX", v.Normal.x}, {"normalY", v.Normal.y}, {"normalZ", v.Normal.z},
							{"texCoordsX", v.TexCoords.x}, {"texCoordsY", v.TexCoords.y}
		};
	}

	void from_json(const nlohmann::json& j, Vertex& v)
	{
		j.at("positionX").get_to(v.Position.x);
		j.at("positionY").get_to(v.Position.y);
		j.at("positionZ").get_to(v.Position.z);
		j.at("normalX").get_to(v.Normal.x);
		j.at("normalY").get_to(v.Normal.y);
		j.at("normalZ").get_to(v.Normal.z);
		j.at("texCoordsX").get_to(v.TexCoords.x);
		j.at("texCoordsY").get_to(v.TexCoords.y);
	}
}