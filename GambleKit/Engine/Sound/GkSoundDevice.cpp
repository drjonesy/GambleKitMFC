//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	GambleKit Copyright (C) 2014 Ivan Donev
//
//	This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//	contact: gamblekit@gmail.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "GkSoundDevice.h"

#include <AL\alut.h>
#include <AL\al.h>

void GkSoundDevice::Initialize()
{
	alutInit(0, 0);
}

void GkSoundDevice::Close()
{
	alutExit();
}

GkSoundDevice::GkSoundDevice(int volume)
{
	this->volume = volume;
}


GkSoundDevice::~GkSoundDevice(void)
{
}

bool GkSoundDevice::IsPlaying(GkSoundSource *soundSource)
{
	ALenum state;
	alGetSourcei(soundSource->handle, AL_SOURCE_STATE, &state);
	
	return (state == AL_PLAYING);
}

void GkSoundDevice::PlaySource(GkSoundSource *soundSource)
{
	alSourcei (soundSource->handle, AL_LOOPING, AL_FALSE);
	alSourcef (soundSource->handle, AL_GAIN, this->volume / 10.0f);
	alSourcePlay(soundSource->handle);
}

void GkSoundDevice::PlaySourceLooping(GkSoundSource *soundSource)
{
	alSourcei (soundSource->handle, AL_LOOPING, AL_TRUE);
	alSourcePlay(soundSource->handle);
}

void GkSoundDevice::PauseSource(GkSoundSource *soundSource)
{
	alSourcePause(soundSource->handle);
}

void GkSoundDevice::StopSource(GkSoundSource *soundSource)
{
	alSourceStop(soundSource->handle);
}
