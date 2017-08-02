#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\GkGameAnimation.h"
#include "..\..\..\Sound\GkSoundDevice.h"

struct GkGameAnimationsSoundBindingAnimation
{
	GkGameAnimation *animation;
	bool played;
};

class GKDLLCLASS GkGameAnimationsSoundBinding :
	public GkGameEntity
{
public:
	GkSoundDevice *soundDevice;
	GkSoundSource *soundSource;

	int numAnimations;
	GkGameAnimationsSoundBindingAnimation *animations;

	bool looping;

	GkGameAnimationsSoundBinding(GkSoundSource *soundSource, GkSoundDevice *soundDevice, bool looping, int numAnimations, ...);
	~GkGameAnimationsSoundBinding(void);

	void Process(GkGame *game);
};

