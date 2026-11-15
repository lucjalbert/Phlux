#pragma once

#include "Actor.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/MaterialLibrary.h"

#include <json_fwd.hpp>

#include <string>
#include <memory>

namespace Actor {

	class ModelActor : public Actor
	{
	public:
		ModelActor(Renderer::Model model);

		virtual void Render() override;

		virtual const std::string GetTypeName() const override { return "ModelActor"; }
		virtual void Serialize(nlohmann::json& j) const override;

		virtual void GetProperties(PropertyList& list) override;

		const Renderer::Model& GetModel() const { return m_Model; }
		Renderer::MaterialId GetMaterialId() { return m_Material; }
		const Renderer::MaterialId GetMaterialId() const { return m_Material; }
		void SetMaterial(Renderer::MaterialId id) { m_Material = id; }

	private:
		Renderer::Model m_Model;
		Renderer::MaterialId m_Material;
	};
}