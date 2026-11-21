#include "Material.h"

#include "Core/Application.h"
#include "AssetRegistry/ShaderLibrary.h"
#include "Shader.h"

#include <iostream>
#include <memory>
#include <variant>

namespace Renderer {

	Material::Material(std::string name, AssetRegistry::ShaderId id)
		: m_ShaderId(id), m_Name(std::move(name))
	{
	}

	void Material::Use() const
	{
		Core::Application::Get().GetAssetRegistry().GetShaderLibrary().Get(m_ShaderId).Use();
	}

	void Material::GetProperties(Actor::PropertyList& list)
	{
		list.Add("Material Name", m_Name, true);
	}

	const AssetRegistry::ShaderId Material::GetShaderId() const
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