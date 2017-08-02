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

#include "GkGameAnimationButton.h"

//#include "..\..\Utils\GkMaths.h"

GkGameAnimationButton::GkGameAnimationButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace, GkGameEntity *disabledFace, bool autoEnable, int x, int y, int width, int height)
	: GkGameButton(defaultFace, selectedFace, disabledFace, autoEnable, x, y, width, height)
{
	GkGameAnimation *face = dynamic_cast<GkGameAnimation*>(this->defaultFace);
	if (face != 0)
		face->EndAnimation();
	
	face = dynamic_cast<GkGameAnimation*>(this->selectedFace);
	if (face != 0)
		face->EndAnimation();
	
	face = dynamic_cast<GkGameAnimation*>(this->disabledFace);
	if (face != 0)
		face->EndAnimation();
	
	this->lastFace = this->GetCurrentFace();
}


GkGameAnimationButton::~GkGameAnimationButton(void)
{
}

void GkGameAnimationButton::Down()
{
	GkGameButton::Down();

	//GkGameAnimation *currentFace = dynamic_cast<GkGameAnimation*>(this->GetCurrentFace());
	//if ((currentFace != 0) && (!currentFace->active))
	//{
	//	currentFace->ResetAnimation();
	//}
}

GkGameEntity* GkGameAnimationButton::GetCurrentFace()
{
	GkGameAnimation *face = this->GetAnimatingFace();
	if (face != 0)
		return face;

	return GkGameButton::GetCurrentFace();
}

GkGameAnimation* GkGameAnimationButton::GetAnimatingFace()
{
	GkGameAnimation *face = dynamic_cast<GkGameAnimation*>(this->defaultFace);
	if ((face != 0) && (face->active) && (!face->loop))
		return face;

	face = dynamic_cast<GkGameAnimation*>(this->selectedFace);
	if ((face != 0) && (face->active) && (!face->loop))
		return face;

	face = dynamic_cast<GkGameAnimation*>(this->disabledFace);
	if ((face != 0) && (face->active) && (!face->loop))
		return face;

	return 0;
}

void GkGameAnimationButton::Process(GkGame *game)
{
	GkGameEntity *currentFace = this->GetCurrentFace();

	if (currentFace != this->lastFace)
	{
		GkGameAnimation *lastAnimation = dynamic_cast<GkGameAnimation*>(this->lastFace);
		if (lastAnimation != 0)
			lastAnimation->EndAnimation();

		GkGameAnimation *currentAnimation = dynamic_cast<GkGameAnimation*>(currentFace);
		if (currentAnimation != 0)
			currentAnimation->ResetAnimation();
		
		this->lastFace = currentFace;
	}

	GkGameButton::Process(game);
}
