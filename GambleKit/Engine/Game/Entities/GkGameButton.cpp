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

#include "GkGameButton.h"

GkGameButton::GkGameButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace, GkGameEntity *disabledFace, bool autoEnable, int x, int y, int width, int height)
	: GkGameEntity()
{
	this->defaultFace = defaultFace;
	this->selectedFace = selectedFace;
	this->disabledFace = disabledFace;

	this->x = x;
	this->y = y;

	if ((width == 0) && (height == 0))
	{
		if (defaultFace != 0)
		{
			int x1, y1, x2, y2;
			if (defaultFace->GetBoundaries(&x1, &y1, &x2, &y2))
			{
				this->width = x2 - x1;
				this->height = y2 - y1;
			}
		}
	}
	else
	{
		this->width = width;
		this->height = height;
	}

	this->visible = true;
	this->clicked = false;
	this->down = false;
	this->enabled = true;
	this->autoEnable = autoEnable;
	this->pushed = false;
}


GkGameButton::~GkGameButton(void)
{
	if (this->defaultFace != 0)
		delete this->defaultFace;

	if (this->selectedFace != 0)
		delete this->selectedFace;

	if (this->disabledFace != 0)
		delete this->disabledFace;
}


void GkGameButton::Down()
{
	if (!this->IsActive())
		return;

	this->pushed = true;
	this->down = true;
}

void GkGameButton::Up(bool click)
{
	if (!this->IsActive())
		return;
	
	this->pushed = false;
	this->clicked = click;
}

void GkGameButton::MouseDown(int x, int y)
{
	if (this->IsMouseOver(x, y))
		this->Down();
}

void GkGameButton::MouseUp(int x, int y)
{
	this->Up(this->pushed && this->IsMouseOver(x, y));
}

bool GkGameButton::WasDown()
{
	bool result = this->down;
	
	this->clicked = false;
	this->down = false;
	this->read = true;

	return result;
}

bool GkGameButton::IsClicked()
{
	bool result = this->clicked;
	
	this->clicked = false;
	this->down = false;
	this->read = true;

	return result;
}

bool GkGameButton::IsActive()
{
	return this->enabled && this->visible;
}

GkGameEntity* GkGameButton::GetCurrentFace()
{
	if (this->enabled)
	{
		if (this->pushed)
			return this->selectedFace != 0 ? this->selectedFace : this->defaultFace;
		else
			return this->defaultFace;
	}
	else
	{
		return this->disabledFace != 0 ? this->disabledFace : this->defaultFace;	
	}
}

bool GkGameButton::IsMouseOver(int x, int y)
{
	if (this->defaultFace != 0)
	{
		// if a defaultFace is given, check a region, relative to it
		int x1, y1, x2, y2;
		if (defaultFace->GetBoundaries(&x1, &y1, &x2, &y2))
			return (x > x1 + this->x) && (x < x1 + this->x + this->width) && (y > y1 + this->y) && (y < y1 + this->y + this->height);
	}
	else
	{
		// if defaultFace is not given, i.e. the button has no visible part, check its region without relation
		return (x > this->x) && (x < this->x + this->width) && (y > this->y) && (y < this->y + this->height);
	}

	return false;
}

void GkGameButton::Process(GkGame *game)
{
	if (this->autoEnable)
	{
		this->enabled = this->read;
		this->read = false;
	}

	GkGameEntity *face = this->GetCurrentFace();
	if (face != 0)
		face->Process(game);
}

void GkGameButton::Draw(GkGame *game)
{
	if (!visible)
		return;

	GkGameEntity *currentFace = this->GetCurrentFace();

	if (currentFace != 0)
		currentFace->Draw(game);
}
