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

#include "GkGameReelRandomModel.h"

#include "..\..\Utils\GkMaths.h"


GkGameReelRandomModel::GkGameReelRandomModel(int numItems, int numSymbols)
	:GkGameReelModel(numItems)
{
	this->numSymbols = numSymbols;
	this->symbols = new GameReelRandomModelSymbol[numSymbols];

	for (int i = 0; i < numSymbols; i++)
		this->SetupSymbol(i, i);
}


GkGameReelRandomModel::~GkGameReelRandomModel(void)
{
	delete[] this->symbols;
}


void GkGameReelRandomModel::SetupSymbol(int symbolIndex, int symbolValue, int hitProbability, int maxHits)
{
	this->symbols[symbolIndex].value = symbolValue;
	this->symbols[symbolIndex].hitProbability = hitProbability;
	this->symbols[symbolIndex].maxHits = maxHits;

	this->overallProbability = 0;
	for (int i = 0; i < this->numSymbols; i++)
		this->overallProbability += this->symbols[i].hitProbability;
}

bool GkGameReelRandomModel::ReplaceReelSymbol(int itemIndex, int symbolIndex, bool forceReplace)
{
	bool autoGenerate = symbolIndex == -1;

	do
	{
		if (autoGenerate)
		{
			symbolIndex = this->GetSymbolIndex();
		}

		if ((this->symbols[symbolIndex].maxHits == GkGameReelRandomModel::UnlimitedHits) || (this->symbols[symbolIndex].occurances < this->symbols[symbolIndex].maxHits) || forceReplace)
		{
			this->symbols[this->items[itemIndex]].occurances--;
			this->items[itemIndex] = symbolIndex;
			this->symbols[symbolIndex].occurances++;
			return true;
		}
	} while (autoGenerate);

	return false;
}

int GkGameReelRandomModel::GetSymbolIndex()
{
	int p = GkMaths::Random(this->overallProbability);

	int s = 0;
	for (int i = 0; i < this->numSymbols; i++)
	{
		s += this->symbols[i].hitProbability;
		if (p < s)
			return i;
	}

	return -1;
}

int GkGameReelRandomModel::GetSymbol()
{
	int symbolIndex = this->GetSymbolIndex();
	if (symbolIndex == -1)
		return -1;
	else
		return this->symbols[symbolIndex].value;
}

void GkGameReelRandomModel::GenerateReel()
{
	for (int symbolIndex = 0; symbolIndex < this->numSymbols; symbolIndex++)
		this->symbols[symbolIndex].occurances = 0;


	for (int itemIndex = 0; itemIndex < this->numItems; itemIndex++)
	{
		for (;;)
		{
			int symbolIndex = this->GetSymbolIndex();
			if ((this->symbols[symbolIndex].maxHits == GkGameReelRandomModel::UnlimitedHits) || (this->symbols[symbolIndex].occurances < this->symbols[symbolIndex].maxHits))
			{
				this->items[itemIndex] = this->symbols[symbolIndex].value;
				this->symbols[symbolIndex].occurances++;
				break;
			}
		}
	}
}

void GkGameReelRandomModel::GenerateReelZero()
{
	this->GenerateReel();
}
