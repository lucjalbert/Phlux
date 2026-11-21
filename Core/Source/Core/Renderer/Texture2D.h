#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>

namespace Renderer {

	struct Texture2DSpecification
	{
		std::string Path;
		uint32_t Width = 0;
		uint32_t Height = 0;

		GLint TilingModeS = GL_REPEAT;
		GLint TilingModeT = GL_REPEAT;
		GLint MinFiltering = GL_LINEAR_MIPMAP_LINEAR;
		GLint MagFiltering = GL_LINEAR;

		GLenum Format = GL_RGB;
		GLint InternalFormat = GL_RGB;
		GLenum Type = GL_UNSIGNED_BYTE;
	};

	class Texture2D
	{
	public:
		Texture2D(Texture2DSpecification Texture2DSpec);
		~Texture2D();

		void Load(GLint level = 0);
		void Reserve();
		void Use(GLenum textureUnit) const;
		void Destroy() const;

		GLuint GetID() const { return m_ID; }
	private:
		GLuint m_ID;

		Texture2DSpecification m_Texture2DSpec;
	};
}
