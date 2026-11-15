#pragma once

#include "ShaderLibrary.h"
#include "Material.h"

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace Renderer {

	struct MaterialId
	{
		uint64_t Value = 0;
		bool operator==(const MaterialId&) const = default;

		static constexpr MaterialId Default() { return MaterialId{ 0 }; }
	};

	class MaterialLibrary
	{
	public:
		MaterialLibrary();

		MaterialId Create(ShaderId shader, std::string name);

		void Destroy(MaterialId id);

		Material& Get(MaterialId id);
		const Material& Get(MaterialId id) const;

		Material& GetDefaultMaterial() { return *m_Default; }
		const Material& GetDefaultMaterial() const { return *m_Default; }

		const uint32_t GetMaterialCount() { return static_cast<uint32_t>(m_Materials.size()); }

		std::vector<MaterialId> GetAllMaterialIds() const;

	private:
		uint64_t m_NextId = 1;
		std::unordered_map<uint64_t, std::unique_ptr<Material>> m_Materials;
		std::unique_ptr<Material> m_Default;
	};
}