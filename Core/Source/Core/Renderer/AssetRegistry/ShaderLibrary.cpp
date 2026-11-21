#include "ShaderLibrary.h"

#include "Core/Renderer/Shader.h"

#include <memory>
#include <assert.h>

namespace AssetRegistry {

	ShaderLibrary::ShaderLibrary()
	{
		m_Shaders[(size_t)ShaderId::Default] = std::make_unique<Renderer::Shader>("Shaders/default.vert", "Shaders/default.frag");
		m_Shaders[(size_t)ShaderId::Unlit] = std::make_unique<Renderer::Shader>("Shaders/unlit.vert", "Shaders/unlit.frag");

		for (const auto& shader : m_Shaders)
			assert(shader && "An ID has no valid shader associated to it");
	}
}