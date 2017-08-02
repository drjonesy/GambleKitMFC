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

#include "GkGameButtonsBinding.h"


GkGameButtonsBinding::GkGameButtonsBinding(int numGameButtons, ...)
{
	GkGameButton **parameters = (GkGameButton **)((int)&numGameButtons + 4);
	this->numGameButtons = numGameButtons;
	this->gameButtons = new	GkGameButton*[numGameButtons];
	for (int i = 0; i < numGameButtons; i++)
		this->gameButtons[i] = parameters[i];

	this->enabled = true;

	this->pushedValue = false;
	this->clickedValue = false;
}


GkGameButtonsBinding::~GkGameButtonsBinding(void)
{
	delete this->gameButtons;
}

void GkGameButtonsBinding::SyncPush(int syncButtonIndex)
{
	GkGameButton *syncButton = this->gameButtons[syncButtonIndex];

	for (int buttonIndex = 0; buttonIndex < this->numGameButtons; buttonIndex++)
	{
		if (buttonIndex != syncButtonIndex)
		{
			GkGameButton *button = this->gameButtons[buttonIndex];

			if (button->IsActive())
			{
				if (button->pushed != syncButton->pushed)
				{
					if (syncButton->pushed)
						button->Down();				
					else
						button->Up(true);
				}
			}
		}
	}

	this->pushedValue = syncButton->pushed;
}

void GkGameButtonsBinding::SyncClick(int syncButtonIndex)
{
	GkGameButton *syncButton = this->gameButtons[syncButtonIndex];

	for (int buttonIndex = 0; buttonIndex < this->numGameButtons; buttonIndex++)
	{
		if (buttonIndex != syncButtonIndex)
		{
			GkGameButton *button = this->gameButtons[buttonIndex];

			if (button->IsActive())
			{
				button->clicked = syncButton->clicked;
			}
		}
	}

	this->clickedValue = syncButton->clicked;
}

void GkGameButtonsBinding::Process(GkGame *game)
{
	if (!enabled)
		return;

	for (int buttonIndex = 0; buttonIndex < this->numGameButtons; buttonIndex++)
	{
		GkGameButton *button = this->gameButtons[buttonIndex];
		if (button->IsActive())
		{
			if (button->pushed != this->pushedValue)
			{
				this->SyncPush(buttonIndex);
				break;
			}
		}
	}

	for (int buttonIndex = 0; buttonIndex < this->numGameButtons; buttonIndex++)
	{
		GkGameButton *button = this->gameButtons[buttonIndex];
		if (button->IsActive())
		{
			if (button->clicked != this->clickedValue)
			{
				// SyncClick is required beside SyncPush, because mouse events execute separately from the Process sequence and MouseUp usually sets a button pushed to false, which actually prevents SyncPush to execute mouse up (for it checks if the button is pushed)
				this->SyncClick(buttonIndex);
				break;
			}
		}
	}


}