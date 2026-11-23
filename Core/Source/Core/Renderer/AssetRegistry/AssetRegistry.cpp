#include "AssetRegistry.h"

#include "Core/Application.h"
#include "ShaderLibrary.h"
#include "MaterialLibrary.h"
#include "ModelLibrary.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"

#include <glm/glm.hpp>

#include <memory>

namespace AssetRegistry {

	AssetRegistry::AssetRegistry()
	{
		m_ShaderLibrary = ShaderLibrary();
		m_MaterialLibrary = MaterialLibrary();
		m_ModelLibrary = ModelLibrary();

		m_ThumbnailCamera = Camera::FlyCamera();

		glGenFramebuffers(1, &m_ThumbnailFBO);
		glGenRenderbuffers(1, &m_ThumbnailDepthRBO);

		m_ThumbnailTextureSettings = Renderer::Texture2DSpecification();
		m_ThumbnailTextureSettings.Width = static_cast<uint32_t>(m_ThumbnailResolution.x);
		m_ThumbnailTextureSettings.Height = static_cast<uint32_t>(m_ThumbnailResolution.y);
	}

	Renderer::Texture2D* AssetRegistry::GenerateAssetThumbnail(ModelId modelId, MaterialId materialId, AssetType assetType)
	{
		const auto& model = m_ModelLibrary.Get(modelId);
		const auto& material = m_MaterialLibrary.Get(materialId);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ThumbnailFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_ThumbnailDepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_ThumbnailResolution.x), static_cast<GLsizei>(m_ThumbnailResolution.y));
		glViewport(0, 0, static_cast<GLsizei>(m_ThumbnailResolution.x), static_cast<GLsizei>(m_ThumbnailResolution.y));

		std::unique_ptr<Renderer::Texture2D> thumbnailTexture = std::make_unique<Renderer::Texture2D>(m_ThumbnailTextureSettings);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, thumbnailTexture->GetID(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ThumbnailDepthRBO);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto& shader = Core::Application::Get().GetAssetRegistry().GetShaderLibrary().Get(material.GetShaderId());
		shader.Use();
		material.ApplyParams(shader);
		shader.SetUniform("view", m_ThumbnailCamera.GetViewMatrix());
		shader.SetUniform("projection", m_ThumbnailCamera.GetProjectionMatrix(glm::vec2(m_ThumbnailResolution.x, m_ThumbnailResolution.y)));
		shader.SetUniform("model", glm::mat4(1.0f));

		model.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, static_cast<GLsizei>(Core::Application::Get().GetViewportBufferSize().x), static_cast<GLsizei>(Core::Application::Get().GetViewportBufferSize().y));

		assert(assetType == AssetType::MODEL || assetType == AssetType::MATERIAL);
		if (assetType == AssetType::MODEL)
		{
			m_ModelThumbnails.emplace(modelId.Value, std::move(thumbnailTexture));
			return m_ModelThumbnails.find(modelId.Value)->second.get();
		}
		else
		{
			m_MaterialThumbnails.emplace(materialId.Value, std::move(thumbnailTexture));
			return m_MaterialThumbnails.find(materialId.Value)->second.get();
		}
	}

	Renderer::Texture2D* AssetRegistry::GetModelThumbnail(ModelId modelId)
	{
		const auto& thumbnailEntry = m_ModelThumbnails.find(modelId.Value);
		if (thumbnailEntry == m_ModelThumbnails.end())
			return nullptr;
		else
			return thumbnailEntry->second.get();
	}

	Renderer::Texture2D* AssetRegistry::GetMaterialThumbnail(MaterialId materialId)
	{
		const auto& thumbnailEntry = m_MaterialThumbnails.find(materialId.Value);
		if (thumbnailEntry == m_MaterialThumbnails.end())
			return nullptr;
		else
			return thumbnailEntry->second.get();
	}
}