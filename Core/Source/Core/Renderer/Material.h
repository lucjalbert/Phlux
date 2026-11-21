#pragma once

#include "AssetRegistry/ShaderLibrary.h"
#include "Core/Scene/Actors/Actor.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <variant>
#include <unordered_map>

namespace Renderer {

	using UniformValue = std::variant<bool, int, float, glm::vec4, glm::mat4>;

	class Material
	{
	public:
		Material(std::string name, AssetRegistry::ShaderId id = AssetRegistry::ShaderId::Default);

		void SetBool(const std::string& name, bool v)				{ m_Params[name] = v; }
		void SetInt(const std::string& name, int v)					{ m_Params[name] = v; }
		void SetFloat(const std::string& name, float v)				{ m_Params[name] = v; }
		void SetVec4(const std::string& name, const glm::vec4 v)	{ m_Params[name] = v; }
		void SetMat4(const std::string& name, const glm::mat4 v)	{ m_Params[name] = v; }

		void ApplyParams(const Shader& shader) const;

		void Use() const;

		std::string& GetName() { return m_Name; }
		const std::string& GetName() const { return m_Name; }
		const AssetRegistry::ShaderId GetShaderId() const;

		void GetProperties(Actor::PropertyList& list);

	private:
		std::string m_Name;
		AssetRegistry::ShaderId m_ShaderId;
		std::unordered_map<std::string, UniformValue> m_Params;
	};
}