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

#include "GkGameAnimationsSoundBinding.h"


GkGameAnimationsSoundBinding::GkGameAnimationsSoundBinding(GkSoundSource *soundSource, GkSoundDevice *soundDevice, bool looping, int numAnimations, ...)
	: GkGameEntity()
{
	this->soundSource = soundSource;
	this->soundDevice = soundDevice;
	this->looping = looping;

	this->numAnimations = numAnimations;
	GkGameAnimation **parameters = (GkGameAnimation **)((int)&numAnimations + 4);
	this->animations = new GkGameAnimationsSoundBindingAnimation[numAnimations];
	for (int i = 0; i < numAnimations; i++)
	{
		this->animations[i].animation = parameters[i];
		this->animations[i].played = false;
	}
}


GkGameAnimationsSoundBinding::~GkGameAnimationsSoundBinding(void)
{
	delete[] this->animations;
}

void GkGameAnimationsSoundBinding::Process(GkGame *game)
{
	for (int i = 0; i < this->numAnimations; i++)
	{
		if (this->animations[i].animation->visible && this->animations[i].animation->active)
		{
			if (!this->animations[i].played)
			{
				if (this->looping)
					this->soundDevice->PlaySourceLooping(this->soundSource);
				else
					this->soundDevice->PlaySource(this->soundSource);

				this->animations[i].played = true;
			}
		}
		else
		{
			if (this->looping)
			{
				this->soundDevice->PauseSource(this->soundSource);
			}

			this->animations[i].played = false;
		}
	}
}