#pragma once

#include "Core/Renderer/Model.h"

#include <json.hpp>

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace AssetRegistry {

	struct ModelId
	{
		uint64_t Value = 0;
		bool operator==(const ModelId&) const = default;

		static constexpr ModelId Default() { return ModelId{ 0 }; }
	};

	class ModelLibrary
	{
	public:
		ModelLibrary();

		ModelId Create(std::string path);
		ModelId Add(ModelId id, std::unique_ptr<Renderer::Model> model);

		void Destroy(ModelId id);
		void Clear();

		Renderer::Model& Get(ModelId id);
		const Renderer::Model& Get(ModelId id) const;

		const uint32_t GetModelCount() { return static_cast<uint32_t>(m_Models.size()); }

		std::vector<ModelId> GetAllModelIds() const;

		void Serialize(nlohmann::json& j) const;
		void Deserialize(const nlohmann::json& j);

	private:
		std::unordered_map<uint64_t, std::unique_ptr<Renderer::Model>> m_Models;
		uint64_t m_NextId = 0;
	};
}