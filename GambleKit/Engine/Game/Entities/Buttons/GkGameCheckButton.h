#pragma once

#include "..\..\..\GkDefinitions.h"

#include "GkGameAnimationButton.h"

class GKDLLCLASS GkGameCheckButton :
	public GkGameAnimationButton
{
public:
	bool checked;

	GkGameCheckButton::GkGameCheckButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace = 0, GkGameEntity *disabledFace = 0, bool autoEnable = true, int x = 0, int y = 0, int width = 0, int heght = 0);

	void GkGameCheckButton::Down();
	GkGameEntity* GkGameCheckButton::GetCurrentFace();

	void GkGameCheckButton::SynchWith(GkGameCheckButton *button);
};

