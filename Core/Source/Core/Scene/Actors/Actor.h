#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Actor {

	enum class PropertyType
	{
		BOOL,
		STRING,
		INT,
		FLOAT,
		VEC2,
		VEC3,
		VEC4
	};

	struct Property
	{
		std::string Name;
		PropertyType Type;
		void* Value;
		bool bReadOnly = false;
	};

	class PropertyList
	{
	public:
		void Add(const std::string& name, bool& value, bool readOnly = false)			{ m_Properties.push_back(Property(name, PropertyType::BOOL, &value, readOnly)); }
		void Add(const std::string& name, std::string& value, bool readOnly = false)	{ m_Properties.push_back(Property(name, PropertyType::STRING, &value, readOnly)); }
		void Add(const std::string& name, int& value, bool readOnly = false)			{ m_Properties.push_back(Property(name, PropertyType::INT, &value, readOnly)); }
		void Add(const std::string& name, float& value, bool readOnly = false)			{ m_Properties.push_back(Property(name, PropertyType::FLOAT, &value, readOnly)); }
		void Add(const std::string& name, glm::vec2& value, bool readOnly = false)		{ m_Properties.push_back(Property(name, PropertyType::VEC2, &value, readOnly)); }
		void Add(const std::string& name, glm::vec3& value, bool readOnly = false)		{ m_Properties.push_back(Property(name, PropertyType::VEC3, &value, readOnly)); }
		void Add(const std::string& name, glm::vec4& value, bool readOnly = false)		{ m_Properties.push_back(Property(name, PropertyType::VEC4, &value, readOnly)); }

		const std::vector<Property>& All() const { return m_Properties; }
	private:
		std::vector<Property> m_Properties;
	};

	class Actor
	{
	public:
		Actor(std::string name);
		virtual ~Actor() = default;
		virtual void Render() {};

		virtual std::string GetName() { return m_Name; }
		virtual void GetProperties(PropertyList& list);

		glm::mat4 GetModelMatrix() const;

		bool bSelected = false;

		bool operator==(const Actor& other) const
		{
			return this->m_Name == other.m_Name;
		}
	protected:
		std::string m_Name;

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	};
}