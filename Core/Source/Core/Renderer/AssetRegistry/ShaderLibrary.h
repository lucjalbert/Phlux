#pragma once

#include "Core/Renderer/Shader.h"

#include <array>

namespace AssetRegistry {

	enum class ShaderId : uint8_t
	{
		Default,
		Unlit,
		Count
	};

	class ShaderLibrary
	{
	public:
		ShaderLibrary();

		Renderer::Shader& Get(ShaderId id) { return *m_Shaders[(size_t)id]; }
		const Renderer::Shader& Get(ShaderId id) const { return *m_Shaders[(size_t)id]; }

	private:
		std::array<std::unique_ptr<Renderer::Shader>, (size_t)ShaderId::Count> m_Shaders;
	};
}