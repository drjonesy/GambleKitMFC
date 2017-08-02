#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\GkGameButton.h"
#include "..\GkGameAnimation.h"

class GKDLLCLASS GkGameAnimationButton :
	public GkGameButton
{
public:
	GkGameAnimationButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace = 0, GkGameEntity *disabledFace = 0, bool autoEnable = true, int x = 0, int y = 0, int width = 0, int heght = 0);
	~GkGameAnimationButton(void);

	GkGameAnimation* GetAnimatingFace();
	GkGameEntity *lastFace;

	virtual void Down();
	virtual GkGameEntity* GetCurrentFace();

	void Process(GkGame *game);
};

