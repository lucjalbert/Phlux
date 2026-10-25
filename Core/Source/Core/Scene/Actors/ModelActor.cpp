#include "ModelActor.h"

#include "Core/Renderer/Model.h"

namespace Actor {

	ModelActor::ModelActor(Renderer::Model model)
		: Actor(model.GetName()), m_Model(model)
	{
	}

	void ModelActor::Render()
	{
		m_Model.Draw();
	}
}