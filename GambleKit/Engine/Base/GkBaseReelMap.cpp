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

#include "GkBaseReelMap.h"

#include "..\OSSpecific\GkOSUtils.h"

GkBaseReelMap::GkBaseReelMap(int numReels, int numReelItems)
	: GkBase()
{
	this->numReels = numReels;
	this->numReelItems = numReelItems;
	
	this->reelMap = new int[this->numReels * this->numReelItems];
	GkOSUtils::ZeroMem(this->reelMap, this->numReels * this->numReelItems * sizeof(int));
}


GkBaseReelMap::~GkBaseReelMap(void)
{
	delete[] reelMap;
}

void GkBaseReelMap::SetSymbol(int reelIndex, int itemIndex, int symbolIndex)
{
 	this->reelMap[reelIndex * this->numReelItems + itemIndex] = symbolIndex;
}

int GkBaseReelMap::GetSymbol(int reelIndex, int itemIndex)
{
	return this->reelMap[reelIndex * this->numReelItems + itemIndex];
}

void GkBaseReelMap::SetItems(GkBaseReelMap *reelMap, int startReel, int startItem, int numReels, int numItems)
{
	int maxReel = startReel + numReels;
	int maxItem = startItem + numItems;
	for (int reelIndex = startReel; reelIndex < maxReel; reelIndex++)
	{
		for (int itemIndex = startItem; itemIndex < maxItem; itemIndex++)
		{
			this->SetSymbol(reelIndex, itemIndex, reelMap->GetSymbol(reelIndex, itemIndex));
		}
	}
}

void GkBaseReelMap::SetItems(GkBaseReelMap *reelMap)
{
	this->SetItems(reelMap, 0, 0, this->numReels, this->numReelItems);
}

void GkBaseReelMap::SetReelItems(GkBaseReelMap *reelMap, int reelIndex)
{
	this->SetItems(reelMap, reelIndex, 0, 1, this->numReelItems);
}

void GkBaseReelMap::SetItems(const int *reelMap)
{
	GkOSUtils::CopyMem(this->reelMap, (void*)reelMap, this->numReels * this->numReelItems * sizeof(int));
}

void GkBaseReelMap::WriteItems(const int *reelMap)
{
	GkOSUtils::CopyMem((void*)reelMap, this->reelMap, this->numReels * this->numReelItems * sizeof(int));
}

int	GkBaseReelMap::CountItems(int reelIndex, int itemIndex)
{
	int result = 0;
	for (int i = 0; i < this->numReelItems; i++)
		if (this->GetSymbol(reelIndex, i) == itemIndex)
			result++;

	return result;
}

int	GkBaseReelMap::CountItems(int itemIndex)
{
	int result = 0;
	for (int i = 0; i < this->numReels; i++)
		result += this->CountItems(i, itemIndex);

	return result;
}

int GkBaseReelMap::FindFirstSymbol(int reelIndex, int symbolIndex)
{
	for (int i = 0; i < this->numReelItems; i++)
		if (this->GetSymbol(reelIndex, i) == symbolIndex)
			return i;

	return -1;
}

bool GkBaseReelMap::ReelEquals(GkBaseReelMap *reelMap, int reelIndex)
{
	if (this->numReelItems != reelMap->numReelItems)
		return false;

	for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
		if (this->GetSymbol(reelIndex, itemIndex) != reelMap->GetSymbol(reelIndex, itemIndex))
			return false;

	return true;
}

bool GkBaseReelMap::ReelsEquals(GkBaseReelMap *reelMap)
{
	if (this->numReels != reelMap->numReels)
		return false;

	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
		if (!this->ReelEquals(reelMap, reelIndex))
			return false;

	return true;
}
