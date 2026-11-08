#include "ModelSerialization.h"

#include "Mesh.h"
#include "Model.h"
#include "Core/Utils/JsonUtils.h"

#include <json.hpp>

#include <vector>

namespace {

	static_assert(sizeof(Renderer::Vertex) == 32);

	template<typename T>
	nlohmann::json ToBinary(const std::vector<T>& source)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		std::vector<std::uint8_t> bytes(source.size() * sizeof(T));
		if (!source.empty())
		{
			std::memcpy(bytes.data(), source.data(), bytes.size());
		}
		return nlohmann::json::binary(std::move(bytes));
	}

	template<typename T>
	std::vector<T> FromBinary(const nlohmann::json& j)
	{
		const auto& bytes = j.get_binary();
		if (bytes.size() % sizeof(T) != 0)
		{
			throw std::runtime_error("Invalid mesh");
		}
		std::vector<T> destination(bytes.size() / sizeof(T));
		if (!destination.empty())
		{
			std::memcpy(destination.data(), bytes.data(), bytes.size());
		}
		return destination;
	}
}

nlohmann::json ModelToJson(const Renderer::Model& model)
{
	nlohmann::json meshesJson = nlohmann::json::array();
	for (const auto& mesh : model.GetMeshes())
	{
		nlohmann::json meshJson;
		meshJson[JsonKeys::Vertices] = ToBinary(mesh->GetVertices());
		meshJson[JsonKeys::Indices] = ToBinary(mesh->GetIndices());
		meshesJson.push_back(std::move(meshJson));
	}
	return { {JsonKeys::ModelName, model.GetName()}, {JsonKeys::Meshes, std::move(meshesJson)} };
}

Renderer::Model ModelFromJson(const nlohmann::json& j)
{
	std::vector<std::unique_ptr<Renderer::Mesh>> meshes;
	for (const auto& meshJson : j.at(JsonKeys::Meshes))
	{
		meshes.push_back(std::make_unique<Renderer::Mesh>(
			FromBinary<Renderer::Vertex>(meshJson.at(JsonKeys::Vertices)),
			FromBinary<uint32_t>(meshJson.at(JsonKeys::Indices))));
	}
	return Renderer::Model(j.at(JsonKeys::ModelName).get<std::string>(), std::move(meshes));
}