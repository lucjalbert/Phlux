#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Actor {

	class Actor
	{
	public:
		Actor(std::string name);
		virtual ~Actor() = default;
		virtual void Render() {};

		virtual std::string GetName() { return m_Name; }
	protected:
		std::string m_Name;
	};
}