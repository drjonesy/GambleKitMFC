#pragma once

#include "..\..\..\GkDefinitions.h"

#include "GkGameAnimationButton.h"


class GKDLLCLASS GkGameClickButton :
	public GkGameAnimationButton
{
public:
	GkGameClickButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace = 0, GkGameEntity *disabledFace = 0, bool autoEnable = true, int x = 0, int y = 0, int width = 0, int height = 0);
	~GkGameClickButton();
};

