#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>

namespace Renderer {

	struct Texture2DSpecification
	{
		std::string path;
		uint32_t width = 0;
		uint32_t height = 0;

		GLint tilingModeS = GL_REPEAT;
		GLint tilingModeT = GL_REPEAT;
		GLint minFiltering = GL_LINEAR_MIPMAP_LINEAR;
		GLint magFiltering = GL_LINEAR;

		GLenum format = GL_RGB;
		GLint internalFormat = GL_RGB;
		GLenum type = GL_UNSIGNED_BYTE;
	};

	class Texture2D
	{
	public:
		Texture2D(Texture2DSpecification Texture2DSpec);
		~Texture2D();

		void Load(GLint level = 0);
		void Use(GLenum textureUnit) const;
		void Destroy() const;

	private:
		GLuint m_ID;

		Texture2DSpecification m_Texture2DSpec;
	};
}
