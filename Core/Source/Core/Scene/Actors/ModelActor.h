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

	private:
		Renderer::Model m_Model;
	};
}