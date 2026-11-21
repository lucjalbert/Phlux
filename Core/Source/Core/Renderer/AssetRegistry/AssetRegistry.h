#pragma once

#include "ShaderLibrary.h"
#include "MaterialLibrary.h"
#include "ModelLibrary.h"
#include "Core/Renderer/Texture2D.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"
#include "Core/Camera/FlyCamera.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>

namespace AssetRegistry {

	enum class AssetType
	{
		MODEL,
		MATERIAL,
		TEXTURE
	};

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

		Renderer::Texture2D* GenerateAssetThumbnail(ModelId modelId, MaterialId materialId, AssetType assetType);

		Renderer::Texture2D* GetModelThumbnail(ModelId modelId);
		Renderer::Texture2D* GetMaterialThumbnail(MaterialId materialId);

	private:
		ShaderLibrary m_ShaderLibrary;
		MaterialLibrary m_MaterialLibrary;
		ModelLibrary m_ModelLibrary;

		GLuint m_ThumbnailFBO;
		GLuint m_ThumbnailDepthRBO;
		glm::vec2 m_ThumbnailResolution = glm::vec2(256.0f, 256.0f);
		Renderer::Texture2DSpecification m_ThumbnailTextureSettings;
		Camera::FlyCamera m_ThumbnailCamera;

		std::unordered_map<uint64_t, std::unique_ptr<Renderer::Texture2D>> m_ModelThumbnails;
		std::unordered_map<uint64_t, std::unique_ptr<Renderer::Texture2D>> m_MaterialThumbnails;
	};
}