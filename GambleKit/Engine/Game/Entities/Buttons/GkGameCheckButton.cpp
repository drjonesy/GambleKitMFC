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

#include "GkGameCheckButton.h"

GkGameCheckButton::GkGameCheckButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace, GkGameEntity *disabledFace, bool autoEnable, int x, int y, int width, int heght)
	: GkGameAnimationButton(defaultFace, selectedFace, disabledFace, autoEnable, x, y, width, heght)
{
	this->checked = false;
}

void GkGameCheckButton::Down()
{
	GkGameAnimationButton::Down();

	if (!this->IsActive())
		return;

	this->checked = !this->checked;
}

GkGameEntity* GkGameCheckButton::GetCurrentFace()
{
	GkGameAnimation *face = this->GetAnimatingFace();
	if (face != 0)
		return face;

	//if (this->enabled)
	//{
		if (this->checked)
		{
			return this->selectedFace != 0 ? this->selectedFace : this->defaultFace;
		}
		else
		{
			return this->defaultFace;
		}
	//}
	//else
	//{
	//	return this->disabledFace != 0 ? this->disabledFace : this->defaultFace;
	//}
}
