#pragma once

#include "..\GkDefinitions.h"

#include <AL\al.h>

#include "..\Base\GkBase.h"

class GKDLLCLASS GkSoundSource
	: public GkBase
{
public:
	ALuint handle, buffer;

	GkSoundSource::GkSoundSource(void);
	GkSoundSource::~GkSoundSource(void);

	void GkSoundSource::LoadFromMemory(void *data);
	void GkSoundSource::UnloadBuffer();
};