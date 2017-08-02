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

#include "GkGameReelsCombinationGenerator.h"

#include "..\Utils\GkMaths.h"

GkGameReelsCombinationGenerator::GkGameReelsCombinationGenerator(int numReels, int numReelItems)
	: GkBaseReelMap(numReels, numReelItems)
{
	this->reels = new GameReelsCombinationGeneratorReel[this->numReels];
	for (int i = 0; i < this->numReels; i++)
	{
		this->reels[i].model = 0;
		this->reels[i].blocked = false;
	}
}


GkGameReelsCombinationGenerator::~GkGameReelsCombinationGenerator(void)
{
	for (int i = 0; i < this->numReels; i++)
	{
		if (this->reels[i].model != 0)
			delete this->reels[i].model;
	}

	delete[] this->reels;
}

void GkGameReelsCombinationGenerator::SetReelModel(int reelIndex, GkGameReelModel *reelModel, bool blocked)
{
	this->reels[reelIndex].model = reelModel;
	this->reels[reelIndex].blocked = blocked;
}


void GkGameReelsCombinationGenerator::GenerateReel(int reelIndex)
{
	this->reels[reelIndex].model->GenerateReel();

	for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
	{
		this->SetSymbol(reelIndex, itemIndex, this->reels[reelIndex].model->items[itemIndex]);
	}
}

void GkGameReelsCombinationGenerator::GenerateCombination()
{
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		if (!this->reels[reelIndex].blocked)
			this->GenerateReel(reelIndex);
	}
}

void GkGameReelsCombinationGenerator::GenerateCombinationZero()
{
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		if (!this->reels[reelIndex].blocked)
		{
			this->reels[reelIndex].model->GenerateReelZero();

			for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
			{
				this->SetSymbol(reelIndex, itemIndex, this->reels[reelIndex].model->items[itemIndex]);
			}		
		}
	}
}
