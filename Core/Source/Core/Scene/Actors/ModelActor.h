#pragma once

#include "Actor.h"
#include "Core/Renderer/Model.h"

#include <json_fwd.hpp>

#include <string>

namespace Actor {

	class ModelActor : public Actor
	{
	public:
		ModelActor(Renderer::Model model);

		virtual void Render() override;

		virtual const std::string GetTypeName() const override { return "ModelActor"; }
		virtual void Serialize(nlohmann::json& j) const override;

		const Renderer::Model& GetModel() const { return m_Model; }

	private:
		Renderer::Model m_Model;
	};
}