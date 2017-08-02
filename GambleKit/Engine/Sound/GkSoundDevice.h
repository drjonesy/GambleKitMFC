#pragma once

#include "..\GkDefinitions.h"

#include "GkSoundSource.h"

class GKDLLCLASS GkSoundDevice
{
public:
	int volume;

	static void Initialize();
	static void Close();

	GkSoundDevice(int volume = 100);
	~GkSoundDevice(void);

	bool IsPlaying(GkSoundSource *soundSource);	
	void PlaySource(GkSoundSource *soundSource);
	void PlaySourceLooping(GkSoundSource *soundSource);
	void PauseSource(GkSoundSource *soundSource);
	void StopSource(GkSoundSource *soundSource);
};