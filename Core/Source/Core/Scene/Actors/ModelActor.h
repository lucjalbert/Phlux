#pragma once

#include "Actor.h"
#include "Core/Renderer/Model.h"

#include <string>

namespace Actor {

	class ModelActor : public Actor
	{
	public:
		ModelActor(Renderer::Model model);

		virtual void Render() override;

		const Renderer::Model& GetModel() const { return m_Model; }
	private:
		Renderer::Model m_Model;
	};
}