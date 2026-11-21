#include "MaterialLibrary.h"

#include <algorithm>
#include <vector>

namespace AssetRegistry {

	MaterialLibrary::MaterialLibrary()
	{
		m_Default = std::make_unique<Renderer::Material>("Default Material", ShaderId::Default());
	}

	MaterialId MaterialLibrary::Create(ShaderId shader, std::string name)
	{
		MaterialId id{ m_NextId++ };
		m_Materials.emplace(id.Value, std::make_unique<Renderer::Material>(name, shader));
		return id;
	}

	void MaterialLibrary::Destroy(MaterialId id)
	{
		if (id.Value != 0) m_Materials.erase(id.Value);
	}

	Renderer::Material& MaterialLibrary::Get(MaterialId id)
	{
		auto material = m_Materials.find(id.Value);
		return material != m_Materials.end() ? *material->second : *m_Default;
	}

	const Renderer::Material& MaterialLibrary::Get(MaterialId id) const
	{
		auto material = m_Materials.find(id.Value);
		return material != m_Materials.end() ? *material->second : *m_Default;
	}

	std::vector<MaterialId> MaterialLibrary::GetAllMaterialIds() const
	{
		std::vector<MaterialId> materialIds;
		materialIds.reserve(m_Materials.size() + 1);

		materialIds.push_back(MaterialId::Default());
		for (const auto& [key, material] : m_Materials)
			materialIds.push_back(MaterialId{ key });

		std::sort(materialIds.begin() + 1, materialIds.end(),
			[](MaterialId a, MaterialId b) { return a.Value < b.Value; });

		return materialIds;
	}
}