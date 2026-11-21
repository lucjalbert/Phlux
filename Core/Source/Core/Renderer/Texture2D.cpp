#include "Texture2D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <iostream>

namespace Renderer {

	Texture2D::Texture2D(Texture2DSpecification Texture2DSpec)
		: m_Texture2DSpec(Texture2DSpec)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture2DSpec.TilingModeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture2DSpec.TilingModeT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture2DSpec.MinFiltering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture2DSpec.MagFiltering);

		m_Texture2DSpec.InternalFormat = m_Texture2DSpec.Format;

		if (m_Texture2DSpec.Width > 0 && m_Texture2DSpec.Height > 0 && m_Texture2DSpec.Path.empty())
		{
			Reserve();
		}
	}

	Texture2D::~Texture2D()
	{
		Destroy();
	}

	void Texture2D::Load(GLint level)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(m_Texture2DSpec.Path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexImage2D(GL_TEXTURE_2D, level, m_Texture2DSpec.InternalFormat, width, height, 0, m_Texture2DSpec.Format, m_Texture2DSpec.Type, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	void Texture2D::Reserve()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_Texture2DSpec.InternalFormat, m_Texture2DSpec.Width, m_Texture2DSpec.Height, 0, m_Texture2DSpec.Format, m_Texture2DSpec.Type, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::Use(GLenum textureUnit) const
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture2D::Destroy() const
	{
		glDeleteTextures(1, &m_ID);
	}
}