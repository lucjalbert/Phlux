#pragma once

#include "Core/Layer.h"
#include "Core/Scene/Actors/Actor.h"

#include <memory>

class DetailPanelLayer : public Core::Layer
{
public:
	virtual void OnImGuiRender();

private:
	std::weak_ptr<Actor::Actor> selectedActor;

	float m_SliderSpeed = 0.1f;
};