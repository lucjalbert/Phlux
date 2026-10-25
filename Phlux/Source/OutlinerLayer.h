#pragma once

#include "Core/Layer.h"
#include "Core/Scene/Actors/Actor.h"

class OutlinerLayer : public Core::Layer
{
public:
	void OnImGuiRender();
};