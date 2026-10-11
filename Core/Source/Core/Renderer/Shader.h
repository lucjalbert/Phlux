#pragma once

#include "Core/Renderer/Texture2D.h"

#include <glad/glad.h>

#include <string>
#include <vector>
#include <memory>

namespace Renderer {

	class Shader
	{
	public:
		static constexpr std::size_t MaxTextures = 16;

		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader();

		void Destroy();

		void Use();
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;

		void AddTexture(std::shared_ptr<Texture2D> texture);
		void UseTexture(uint32_t index, GLenum textureUnit);

	private:
		GLuint m_ID;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
	};
}

