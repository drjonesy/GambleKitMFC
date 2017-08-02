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

#include "GkGameReelStripModel.h"

#include "..\..\OSSpecific\GkOSUtils.h"
#include "..\..\Utils\GkMaths.h"

GkGameReelStripModel::GkGameReelStripModel(int numItems, int numSymbols, ...)
	: GkGameReelModel(numItems)
{
	this->numSymbols = numSymbols;
	this->symbolStrip = new int[this->numSymbols];

	int *parameters = (int*)((int)&numSymbols + 4);
	for (int i = 0; i < this->numSymbols; i++)
		this->symbolStrip[i] = parameters[i];

	this->stripPosition = GkMaths::Random(this->numSymbols);
}


GkGameReelStripModel::~GkGameReelStripModel(void)
{
	delete[] this->symbolStrip;
}

int GkGameReelStripModel::GetSymbol()
{
	this->stripPosition = (this->stripPosition + 1) % this->numSymbols;
	return this->symbolStrip[this->stripPosition];
}

void GkGameReelStripModel::GenerateReel()
{
	this->stripPosition = GkMaths::Random(this->numSymbols);

	for (int itemIndex = 0; itemIndex < this->numItems; itemIndex++)
	{
		this->items[itemIndex] = this->GetSymbol();
	}
}

void GkGameReelStripModel::GenerateReelZero()
{
	this->stripPosition = this->numSymbols - this->numItems / 2;

	for (int itemIndex = 0; itemIndex < this->numItems; itemIndex++)
	{
		this->items[itemIndex] = this->GetSymbol();
	}
}