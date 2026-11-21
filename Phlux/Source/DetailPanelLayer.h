#pragma once

#include "Core/Layer.h"
#include "Core/Scene/Actors/Actor.h"

#include <memory>

class DetailPanelLayer : public Core::Layer
{
public:
	virtual void OnImGuiRender();

private:
	float m_SliderSpeed = 0.1f;

	void SetupMaterialComboMenu(const char* menuLabel);
	void SetupModelComboMenu(const char* menuLabel);
};