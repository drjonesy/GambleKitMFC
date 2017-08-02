#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameEntity.h"
#include "..\..\Graphics\GkAnimatedTexture.h"

class GKDLLCLASS GkGameAnimation : 
	public GkGameEntity
{
public:
	GkAnimatedTexture *animatedTexture;

	int x, y, width, height;
	bool visible, active, loop;
	float framesPerSecond, currentFrame;

	bool flipX, flipY;


	GkGameAnimation::GkGameAnimation(GkAnimatedTexture *animatedTexture = 0, int x = 0, int y = 0, bool active = true, bool loop = true, bool visible = true, bool flipX = false, bool flipY = false, float framesPerSecond = 0.0f, int width = 0, int height = 0);
	GkGameAnimation::~GkGameAnimation(void);

	void GkGameAnimation::SetAnimatedTexture(GkAnimatedTexture *animatedTexture = 0, float framesPerSecond = 0.0f);
	virtual void GkGameAnimation::Process(GkGame *game);
	virtual void GkGameAnimation::Draw(GkGame *game);
	bool GkGameAnimation::GetBoundaries(int *x1, int *y1, int *x2, int*y2);
	void ResetAnimation(bool start = true);
	void EndAnimation();
};

