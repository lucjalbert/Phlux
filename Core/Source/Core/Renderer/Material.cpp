#include "Material.h"

#include "Core/Application.h"
#include "ShaderLibrary.h"
#include "Shader.h"

#include <iostream>
#include <memory>
#include <variant>

namespace Renderer {

	Material::Material(std::string name, ShaderId id)
		: m_ShaderId(id), m_Name(std::move(name))
	{
	}

	void Material::Use() const
	{
		Core::Application::Get().GetShaderLibrary().Get(m_ShaderId).Use();
	}

	void Material::GetProperties(Actor::PropertyList& list)
	{
		list.Add("Material Name", m_Name, true);
	}

	const ShaderId Material::GetShaderId() const
	{
		return m_ShaderId;
	}

	void Material::ApplyParams(const Shader& shader) const
	{
		for (const auto& [name, value] : m_Params)
		{
			std::visit([&](const auto& value)
			{
				shader.SetUniform(name, value);
			}, value);
		}
	}
}