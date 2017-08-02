#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGame.h"
#include "..\Entities\GkGameAnimation.h"
#include "..\..\Graphics\GkTexture.h"

class GKDLLCLASS GkGameStreamingAnimation :
	public GkGameAnimation
{
public:
	GkTexture *texture;

	wchar_t *fileName, *loadExpression;

	bool freeAtEnd;
	int lastRenderedFrame;
	int numFrames; // required only if animation is backwards, i.e. if framesPerSecond is negative

	// Because the animation is streaming and the number of frames is practically unknown, if it is played backwards, i.e. framesPerSecond is negative, then numFrames must be provided
	GkGameStreamingAnimation::GkGameStreamingAnimation(const wchar_t *fileName, const wchar_t *loadExpression, int x = 0, int y = 0, bool freeAtEnd = true, float framesPerSecond = 25.0f, int numFrames = 0);
	GkGameStreamingAnimation::~GkGameStreamingAnimation(void);

	void GkGameStreamingAnimation::SetSource(const wchar_t *fileName, const wchar_t *loadExpression, float framesPerSecond = 25.0f, int numFrames = 0);
	void GkGameStreamingAnimation::Free();
	bool GkGameStreamingAnimation::Load(int index);
	void GkGameStreamingAnimation::Start();
	void GkGameStreamingAnimation::Process(GkGame *game);
	void GkGameStreamingAnimation::Draw(GkGame *game);
};

