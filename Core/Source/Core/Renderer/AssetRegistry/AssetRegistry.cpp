#include "AssetRegistry.h"

#include "Core/Application.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"

namespace AssetRegistry {

	AssetRegistry::AssetRegistry()
	{
		m_ShaderLibrary = ShaderLibrary();
		m_MaterialLibrary = MaterialLibrary();
		m_ModelLibrary = ModelLibrary();
	}

	Renderer::Texture2D AssetRegistry::GenerateAssetThumbnail(const Renderer::Model& model, const Renderer::Material& material)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ThumbnailFBO);
		glViewport(0, 0, static_cast<GLsizei>(m_ThumbnailResolution.x), static_cast<GLsizei>(m_ThumbnailResolution.y));

		Renderer::Texture2D thumbnailTexture = Renderer::Texture2D(Renderer::Texture2DSpecification());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, thumbnailTexture.GetID(), 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, static_cast<GLsizei>(Core::Application::Get().GetFramebufferSize().x), static_cast<GLsizei>(Core::Application::Get().GetFramebufferSize().y));

		return thumbnailTexture;
	}
}