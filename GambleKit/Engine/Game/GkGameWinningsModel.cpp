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

#include "GkGameWinningsModel.h"

#include "..\OSSpecific\GkOSUtils.h"
#include "..\Utils\GkMaths.h"

GkGameWinningsModel::GkGameWinningsModel(int numReels, int numReelItems, int numLines, int numSymbols, int numScatters, int numChannels)
{
	this->numReels = numReels;
	this->numReelItems = numReelItems;
	this->numLines = numLines;
	this->numSymbols = numSymbols;
	this->numScatters = numScatters;
	this->numChannels = numChannels;
	this->selectedChannel = 0;
	
	this->lines = new GameWinningLineModel[this->numLines + numScatters];
	for (int i = 0; i < this->numLines + this->numScatters; i++)
	{
		this->lines[i].active = true;
		this->lines[i].coordinates = new int[this->numReels];
		GkOSUtils::ZeroMem(this->lines[i].coordinates, this->numReels * sizeof(int));
	}

	this->symbols = new GameWinningSymbolModel*[this->numChannels];
	for (int channel = 0; channel < this->numChannels; channel++)
	{
		this->symbols[channel] = new GameWinningSymbolModel[this->numSymbols];
		for (int symbol = 0; symbol < this->numSymbols; symbol++)
		{
			this->symbols[channel][symbol].coefficients = new int[this->numReels];
			GkOSUtils::ZeroMem(this->symbols[channel][symbol].coefficients, this->numReels * sizeof(int));
		}
	}
}

GkGameWinningsModel::~GkGameWinningsModel(void)
{
	for (int channel = 0; channel < this->numChannels; channel++)
	{
		for (int symbol = 0; symbol < this->numSymbols; symbol++)
		{
			delete[] this->symbols[channel][symbol].coefficients;
		}
		delete[] this->symbols[channel];
	}
	delete[] this->symbols;

	for (int i = 0; i < this->numLines + this->numScatters; i++)
	{
		delete[] this->lines[i].coordinates;
	}
	delete[] this->lines;
}

void GkGameWinningsModel::SetupLine(int lineIndex, ...)
{
	//GkOSUtils::CopyMem(this->lines[lineIndex], (void*)lineCoordinates, this->numReels * sizeof(int));

	va_list lineCoordinates;
	va_start(lineCoordinates, lineIndex); 
	for(int i = 0; i < this->numReels; ++i )
		this->lines[lineIndex].coordinates[i] = va_arg(lineCoordinates, int);
	va_end(lineCoordinates);
}

void GkGameWinningsModel::SetupSymbol(int symbolIndex, int channelIndex, unsigned int symbolFlags, unsigned int symbolExclusionFlags, ...)
{
	va_list winCoefficients;
	va_start(winCoefficients, symbolExclusionFlags); 
	for(int i = 0; i < this->numReels; ++i )
	{
		this->symbols[channelIndex][symbolIndex].coefficients[i] = va_arg(winCoefficients, int);
	}
	va_end(winCoefficients);

	this->symbols[channelIndex][symbolIndex].flags = symbolFlags;
	this->symbols[channelIndex][symbolIndex].exclusionFlags = symbolExclusionFlags;
}

int GkGameWinningsModel::GetLineSymbolCoordinate(int reelIndex, int lineIndex)
{
	return this->lines[lineIndex].coordinates[reelIndex];
}

GameWinningSymbolModel *GkGameWinningsModel::GetSymbolModel(int symbol)
{
	return &this->symbols[this->selectedChannel][symbol];
}

int GkGameWinningsModel::GetSymbolCoefficient(int symbol, int symbolCount)
{
	if (symbolCount == 0)
		return 0;

	return this->symbols[this->selectedChannel][symbol].coefficients[symbolCount - 1];
}

int GkGameWinningsModel::CompareSymbolWinnning(int symbol1, int symbol1Count, int symbol2, int symbol2Count)
{
	int coefficientDelta = this->GetSymbolCoefficient(symbol1, symbol1Count) - this->GetSymbolCoefficient(symbol2, symbol2Count);
	if (coefficientDelta != 0)
		return GkMaths::Sign(coefficientDelta);

	for (int numSymbols = GkMaths::Max(symbol1Count, symbol2Count); numSymbols <= this->numReels; numSymbols++)
	{
		coefficientDelta = this->GetSymbolCoefficient(symbol1, numSymbols) - this->GetSymbolCoefficient(symbol2, numSymbols);
		if (coefficientDelta != 0)
			return GkMaths::Sign(coefficientDelta);
	}

	return 0;
}
