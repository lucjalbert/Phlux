#pragma once

#include "Shader.h"

#include <array>

namespace Renderer {

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

		Shader& Get(ShaderId id) { return *m_Shaders[(size_t)id]; }
		const Shader& Get(ShaderId id) const { return *m_Shaders[(size_t)id]; }

	private:
		std::array<std::unique_ptr<Shader>, (size_t)ShaderId::Count> m_Shaders;
	};
}