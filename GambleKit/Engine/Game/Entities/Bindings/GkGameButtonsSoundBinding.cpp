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

#include "GkGameButtonsSoundBinding.h"


GkGameButtonsSoundBinding::GkGameButtonsSoundBinding(GkSoundSource *soundSource, GkSoundDevice *soundDevice, int numButtons, ...)
	: GkGameEntity()
{
	this->soundSource = soundSource;
	this->soundDevice = soundDevice;

	this->numButtons = numButtons;
	GkGameButton **parameters = (GkGameButton **)((int)&numButtons + 4);
	this->buttons = new GameButtonsSoundBindingButton[numButtons];
	for (int i = 0; i < numButtons; i++)
	{
		this->buttons[i].button = parameters[i];
		this->buttons[i].played = false;
	}
}


GkGameButtonsSoundBinding::~GkGameButtonsSoundBinding(void)
{
	delete[] this->buttons;
}

void GkGameButtonsSoundBinding::Process(GkGame *game)
{
	for (int i = 0; i < this->numButtons; i++)
	{
		if (this->buttons[i].button->pushed)
		{
			if (!this->buttons[i].played)
			{
				this->soundDevice->PlaySource(this->soundSource);

				this->buttons[i].played = true;
			}
		}
		else
		{
			this->buttons[i].played = false;
		}
	}
}