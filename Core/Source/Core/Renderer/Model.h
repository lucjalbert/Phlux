#pragma once

#include "Mesh.h"

#include <assimp/scene.h>
#include <json.hpp>

#include <vector>
#include <string>
#include <memory>

namespace Renderer {

	class Model
	{
	public:
		Model(std::string path) { LoadModel(path); }
		Model(std::string name, std::vector<std::unique_ptr<Mesh>> meshes);

		void Draw() const;

		const std::string GetName() const { return m_Name; }
		const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
	
	private:
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		std::string m_Directory;
		std::string m_Name;

		void LoadModel(std::string path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		std::unique_ptr<Mesh> ProcessImportedMesh(aiMesh *mesh, const aiScene *scene);
		std::unique_ptr<Mesh> ProcessLoadedMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	};
}