#pragma once

#include "Actor.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/AssetRegistry/ModelLibrary.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/AssetRegistry/MaterialLibrary.h"

#include <json_fwd.hpp>

#include <string>
#include <memory>

namespace Actor {

	class ModelActor : public Actor
	{
	public:
		ModelActor(AssetRegistry::ModelId modelId);

		virtual void Render() override;

		virtual const std::string GetTypeName() const override { return "ModelActor"; }
		virtual void Serialize(nlohmann::json& j) const override;

		virtual void GetProperties(PropertyList& list) override;

		AssetRegistry::ModelId GetModelId() { return m_Model; }
		const AssetRegistry::ModelId GetModelId() const { return m_Model; }
		void SetModel(AssetRegistry::ModelId id) { m_Model = id; }

		AssetRegistry::MaterialId GetMaterialId() { return m_Material; }
		const AssetRegistry::MaterialId GetMaterialId() const { return m_Material; }
		void SetMaterial(AssetRegistry::MaterialId id) { m_Material = id; }

	private:
		AssetRegistry::ModelId m_Model;
		AssetRegistry::MaterialId m_Material;

		AssetRegistry::ModelLibrary& m_ModelLibrary;
	};
}