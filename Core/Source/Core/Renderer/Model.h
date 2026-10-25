#pragma once

#include "Mesh.h"

#include <assimp/scene.h>

#include <vector>
#include <string>

namespace Renderer {

	class Model
	{
	public:
		Model(std::string path)
		{
			LoadModel(path);
		}
		void Draw();

		std::string GetName() { return m_Name; }
	
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		std::string m_Name;

		void LoadModel(std::string path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	};
}