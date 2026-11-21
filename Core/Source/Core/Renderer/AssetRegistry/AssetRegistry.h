#pragma once

#include "ShaderLibrary.h"
#include "MaterialLibrary.h"
#include "ModelLibrary.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"

#include <glm/glm.hpp>

namespace AssetRegistry {

	class AssetRegistry
	{
	public:
		AssetRegistry();

		ShaderLibrary& GetShaderLibrary() { return m_ShaderLibrary; }
		const ShaderLibrary& GetShaderLibrary() const { return m_ShaderLibrary; }
		MaterialLibrary& GetMaterialLibrary() { return m_MaterialLibrary; }
		const MaterialLibrary& GetMaterialLibrary() const { return m_MaterialLibrary; }
		ModelLibrary& GetModelLibrary() { return m_ModelLibrary; }
		const ModelLibrary& GetModelLibrary() const { return m_ModelLibrary; }

		Renderer::Texture2D GenerateAssetThumbnail(const Renderer::Model& model, const Renderer::Material& material);

	private:
		ShaderLibrary m_ShaderLibrary;
		MaterialLibrary m_MaterialLibrary;
		ModelLibrary m_ModelLibrary;

		GLuint m_ThumbnailFBO;
		glm::vec2 m_ThumbnailResolution;
	};
}