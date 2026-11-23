#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <json.hpp>

namespace nlohmann {

	template<> struct adl_serializer<glm::vec3>
	{
		static void to_json(json& j, const glm::vec3& v) { j = { v.x, v.y, v.z }; }
		static void from_json(const json& j, glm::vec3& v)
		{
			v = { 
				j.at(0).get<float>(),
				j.at(1).get<float>(),
				j.at(1).get<float>()
			};
		}
	};

	template<> struct adl_serializer<glm::mat4>
	{
		static void to_json(json& j, const glm::mat4& m)
		{
			const float* p = glm::value_ptr(m);
			j = std::vector<float>(p, p + 16);
		}
		static void from_json(const json& j, glm::mat4& m)
		{
			if (!j.is_array() || j.size() != 16)
				throw std::runtime_error("mat4 must be an array of 16 numbers");

			float* p = glm::value_ptr(m);
			for (int i = 0; i < 16; ++i)
				p[i] = j.at(i).get<float>();
		}
	};
}

namespace JsonKeys {

	// Scene
	constexpr const char* Version = "Version";
	constexpr const char* Actors = "Actors";

	// Actors
	constexpr const char* ActorType = "ActorType";
	constexpr const char* ActorName = "ActorName";
	constexpr const char* ActorTransform = "ActorTransform";

	// Mesh and model
	constexpr const char* ModelName = "ModelName";
	constexpr const char* Vertices = "Vertices";
	constexpr const char* Indices = "Indices";
	constexpr const char* Meshes = "Meshes";
	constexpr const char* ModelId = "ModelId";
	constexpr const char* ModelData = "ModelData";
	constexpr const char* ModelLibrary = "ModelLibrary";
}