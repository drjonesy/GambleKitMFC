#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\GkGameButton.h"
#include "..\..\..\Sound\GkSoundDevice.h"

struct GameButtonsSoundBindingButton
{
	GkGameButton *button;
	bool played;
};

class GKDLLCLASS GkGameButtonsSoundBinding :
	public GkGameEntity
{
public:
	GkSoundDevice *soundDevice;
	GkSoundSource *soundSource;

	int numButtons;
	GameButtonsSoundBindingButton *buttons;

	GkGameButtonsSoundBinding(GkSoundSource *soundSource, GkSoundDevice *soundDevice, int numButtons, ...);
	~GkGameButtonsSoundBinding(void);

	void Process(GkGame *game);
};

