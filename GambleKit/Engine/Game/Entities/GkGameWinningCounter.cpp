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

#include "GkGameWinningCounter.h"

#include "..\GkGame.h"

GkGameWinningCounter::GkGameWinningCounter(int *actualValue, float minSpeed, float maxTime)
	: GkGameEntity()

{
	this->actualValue = actualValue;
	this->minSpeed = minSpeed;
	this->maxTime = maxTime;

	this->SetValue((float)*this->actualValue, 0.0f);
}


GkGameWinningCounter::~GkGameWinningCounter(void)
{
}

void GkGameWinningCounter::SetValue(float value, float maxTime)
{
	float delta = value - this->displayValue;
	if (maxTime == -1.0f)
		maxTime = this->maxTime;
	
	if (maxTime == 0.0f)
	{
		this->displayValue = value;
	}
	else
	{
		float minSpeed = delta / maxTime;
		this->speed = this->minSpeed > minSpeed ? this->minSpeed : minSpeed;
	}

	this->oldValue = value;
}

void GkGameWinningCounter::Reach(float maxTime)
{
	this->SetValue((float)*this->actualValue, maxTime);
}

bool GkGameWinningCounter::IsCounting()
{
	return this->intDisplayValue != *this->actualValue;
}

void GkGameWinningCounter::Process(GkGame *game)
{
	float value = (float)*this->actualValue;

	if (value != this->oldValue)
		this->SetValue(value, this->maxTime);

	this->displayValue += this->speed * game->timer->tickTime;

	if (this->speed > 0.0f)
	{
		if (this->displayValue > value)
			this->displayValue = value;
	}
	else
	{
		if (this->displayValue < value)
			this->displayValue = value;
	}

	this->intDisplayValue = (int)this->displayValue;

	GkGameEntity::Process(game);
}
