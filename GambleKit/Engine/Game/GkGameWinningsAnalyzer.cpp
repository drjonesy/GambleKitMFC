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

#include "GkGameWinningsAnalyzer.h"

#include "..\Utils\GkMaths.h"
#include "..\OSSpecific\GkOSUtils.h"

#include <stdarg.h>

// Camparison of reel combinations - combinations may have
// A. Exactly the same symbol wins as the compared - equal
// B. The same symbol wins from the compared plus more - greater
// C. Symbol wins from the compared, but not all or with less symbols - lesser
// D. Not all the symbol wins from the compared plus more - different
//
// Rule set
//
// Definition: A symbol win is considered a winning of EXACTLY the same symbol and either on EXACTLY the same line OR BEING A SCATTER
//
// Signs of comparison:
// 1.1 Contains all the symbol wins as the compared one
// 1.2 Contains symbol wins, that are not present in the compared one
//
// In addition to that, there might be symbol win(s) with
// 2.1 The same number of winning items
// 2.2 Greater number of winning items
// 2.3 Lesser number of winning items
//
// Result
// Equal - 1.1 and not 1.2 and not 2.2. and not 2.3
// Greater - 1.1. and (1.2 or 2.2) and not 2.3
// Lesser - (not 1.1 or 2.3) and not 1.2 and not 2.2
// Different - otherwise


GkGameWinningsAnalyzer::GkGameWinningsAnalyzer(GkGameWinningsModel *model)
	: GkBaseReelMap(model->numReels, model->numReelItems)
{
	this->model = model;

	this->comparisonMode = GameWinningAnalyzerComparisonMode_FullComparison;

	this->InitWinningData(&this->winnings);
}

GkGameWinningsAnalyzer::~GkGameWinningsAnalyzer(void)
{
	this->DisposeWinningData(&this->winnings);
}

GameWinning GkGameWinningsAnalyzer::GetSymbolWinning(int symbolIndex, int lineIndex)
{
	GameWinning reelItem;
	reelItem.symbolIndex = symbolIndex;
	reelItem.lineIndex = lineIndex;
	reelItem.numItems = 0;
	reelItem.itemsStart = 0;

	GameWinning result = reelItem;

	GameWinningSymbolModel *symbolModel = this->model->GetSymbolModel(symbolIndex);

	bool sequenceValid = false;
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		int reelSymbolIndex = this->GetLineSymbol(reelIndex, lineIndex);
		GameWinningSymbolModel *reelSymbolModel = this->model->GetSymbolModel(reelSymbolIndex);

		if (this->SymbolsMatch(symbolIndex, reelSymbolIndex))
		{
			if (symbolIndex == reelSymbolIndex)
				sequenceValid = true;

			if (reelItem.numItems == 0)
			{
				reelItem.itemsStart = reelIndex;
				reelItem.symbolsFlags = GkGameWinningsModel::SymbolFlag_None;
			}

			reelItem.numItems++;
			reelItem.symbolsFlags |= reelSymbolModel->flags;

			if (sequenceValid && (reelItem.numItems > result.numItems))
			{
				result = reelItem;
			}
		}
		else
		{
			if ((symbolModel->flags & GkGameWinningsModel::SymbolFlag_AnyPlace) == 0)
				break;

			sequenceValid = false;
			reelItem.numItems = 0;
		}
	}

	result.itemsEnd = result.itemsStart + result.numItems - 1;
	result.winningCoefficient = this->model->GetSymbolCoefficient(result.symbolIndex, result.numItems);

	return result;
}

bool GkGameWinningsAnalyzer::SymbolsMatch(int symbolIndex, int testSymbolIndex)
{
	GameWinningSymbolModel *symbolModel = this->model->GetSymbolModel(symbolIndex),
						   *testSymbolModel = this->model->GetSymbolModel(testSymbolIndex);
	
	return ((testSymbolIndex == symbolIndex) || ((testSymbolModel->flags & GkGameWinningsModel::SymbolFlag_Wild) != 0)) &&
		   ((testSymbolModel->flags & symbolModel->exclusionFlags) == 0);
}

GameWinning GkGameWinningsAnalyzer::GetScatterWinning(int symbolIndex)
{
	int scatterNumber = 0;
	for (int symbolsIndex = 0; symbolsIndex < symbolsIndex; symbolsIndex++)
		if (this->model->GetSymbolModel(symbolsIndex)->flags & GkGameWinningsModel::SymbolFlag_Scatter)
			scatterNumber++;


	GameWinning result;
	result.symbolIndex = symbolIndex;
	result.numItems = 0;
	result.lineIndex = this->model->numLines + scatterNumber;

	if (scatterNumber >= this->model->numScatters)
	{
		result.symbolsFlags = GkGameWinningsModel::SymbolFlag_None;
		result.winningCoefficient = 0;
		return result;
	}

	GameWinningSymbolModel *symbolModel = this->model->GetSymbolModel(symbolIndex);

	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		this->model->lines[result.lineIndex].coordinates[reelIndex] = this->FindFirstSymbol(reelIndex, symbolIndex);
		if (this->model->lines[result.lineIndex].coordinates[reelIndex] != -1)
		{
			if (result.numItems == 0)
				result.itemsStart = reelIndex;

			result.numItems++;
			result.itemsEnd = reelIndex;
		}
	}

	result.symbolsFlags = symbolModel->flags;
	result.winningCoefficient = this->model->GetSymbolCoefficient(result.symbolIndex, result.numItems);

	return result;
}

void GkGameWinningsAnalyzer::AnalyzeWinnings(GkBaseReelMap *reelMap)
{
	if (reelMap != 0)
		this->SetItems(reelMap);

	this->winnings.count = 0;

	// Analyze line winnings
	for (int lineIndex = 0; lineIndex < this->model->numLines; lineIndex++)
	{
		if (this->model->lines[lineIndex].active)
		{
			GameWinning maxLineWin;
			maxLineWin.winningCoefficient = 0;

			for (int symbolIndex = 0; symbolIndex < this->model->numSymbols; symbolIndex++)
			{
				if ((this->model->GetSymbolModel(symbolIndex)->flags & GkGameWinningsModel::SymbolFlag_Scatter) == 0)
				{
					GameWinning symbolWin = this->GetSymbolWinning(symbolIndex, lineIndex);
					if (symbolWin.winningCoefficient != 0)
					{
						if ((maxLineWin.winningCoefficient == 0) || 
							(this->model->CompareSymbolWinnning(symbolWin.symbolIndex, symbolWin.numItems, maxLineWin.symbolIndex, maxLineWin.numItems) > 0))
						{
							maxLineWin = symbolWin;
						}
					}
				}
			}

			if (maxLineWin.winningCoefficient != 0)
			{
				this->winnings.win[this->winnings.count++] = maxLineWin;
			}
		}
	}


	// Analyze scatters
	for (int symbolIndex = 0; symbolIndex < this->model->numSymbols; symbolIndex++)
	{
		if ((this->model->GetSymbolModel(symbolIndex)->flags & GkGameWinningsModel::SymbolFlag_Scatter) != 0)
		{
			GameWinning symbolWin = this->GetScatterWinning(symbolIndex);
			if (symbolWin.winningCoefficient != 0)
			{
				this->winnings.win[this->winnings.count++] = symbolWin;
			}
		}
	}


	// Calculate total winnigs, total flags and manage items with SymbolFlag_IgnoreWinning
	this->totalWinning = 0;
	this->totalFlags = GkGameWinningsModel::SymbolFlag_None;
	for (int i = 0; i < this->winnings.count; i++)
	{
		if (this->winnings.win[i].symbolsFlags & GkGameWinningsModel::SymbolFlag_IgnoreWinning)
			this->winnings.win[i].winningCoefficient = 0;

		this->totalWinning += this->winnings.win[i].winningCoefficient;
		this->totalFlags |= this->winnings.win[i].symbolsFlags;
	}

	this->SortWinnings();
}

int GkGameWinningsAnalyzer::WinningOrder(int winningIndex1, int winningIndex2)
{
	bool win1End = (this->model->GetSymbolModel(this->winnings.win[winningIndex1].symbolIndex)->flags & GkGameWinningsModel::SymbolFlag_AtEnd) != 0;
	bool win2End = (this->model->GetSymbolModel(this->winnings.win[winningIndex2].symbolIndex)->flags & GkGameWinningsModel::SymbolFlag_AtEnd) != 0;
	if (win1End && !win2End)
		return 1;
	
	if (win2End && !win1End)
		return -1;

	return GkMaths::Sign(this->winnings.win[winningIndex1].lineIndex - this->winnings.win[winningIndex2].lineIndex);
}

int GkGameWinningsAnalyzer::CompareWinning(int winningIndex1, int winningIndex2)
{
	return GkMaths::Sign(this->winnings.win[winningIndex1].winningCoefficient - this->winnings.win[winningIndex2].winningCoefficient);
}

void GkGameWinningsAnalyzer::SortWinnings()
{
	for (int i = 0; i < this->winnings.count; i++)
	{
		int leadingWin = i;
		for (int j = i + 1; j < this->winnings.count; j++)
		{
			if (this->WinningOrder(leadingWin, j) > 0)
				leadingWin = j;
		}

		if (leadingWin != i)
		{
			GameWinning t = this->winnings.win[i];
			this->winnings.win[i] = this->winnings.win[leadingWin];
			this->winnings.win[leadingWin] = t;
		}
	}
}

void GkGameWinningsAnalyzer::MultiplyWinnings(int factor)
{
	for (int i = 0; i < this->winnings.count; i++)
		this->winnings.win[i].winningCoefficient *= factor;
	this->totalWinning *= factor;
}

int GkGameWinningsAnalyzer::GetLineSymbol(int reelIndex, int lineIndex)
{
	return this->GetSymbol(reelIndex, model->GetLineSymbolCoordinate(reelIndex, lineIndex));
}

int GkGameWinningsAnalyzer::FindWinning(int symbolIndex, int minSymbols)
{
	for (int i = 0; i < this->winnings.count; i++)
		if ((this->winnings.win[i].symbolIndex == symbolIndex) && (this->winnings.win[i].numItems >= minSymbols))
			return i;

	return -1;
}

bool GkGameWinningsAnalyzer::ReelBelongsToWinning(int reelIndex, int winningIndex)
{
	if ((winningIndex < 0) || (winningIndex >= this->winnings.count))
		return false;

	if (this->winnings.win[winningIndex].symbolsFlags & GkGameWinningsModel::SymbolFlag_Scatter)
	{
		if (this->model->lines[this->winnings.win[winningIndex].lineIndex].coordinates[reelIndex] == -1)
			return false;
	}
	else
	{
		if ((reelIndex < this->winnings.win[winningIndex].itemsStart) || (reelIndex > this->winnings.win[winningIndex].itemsStart + this->winnings.win[winningIndex].numItems - 1))
			return false;
	}

	return true;
}

bool GkGameWinningsAnalyzer::ReelBelongsToAnyWinning(int reelIndex)
{
	for (int winningIndex = 0; winningIndex < this->winnings.count; winningIndex++)
		if (this->ReelBelongsToWinning(reelIndex, winningIndex))
			return true;

	return false;
}

bool GkGameWinningsAnalyzer::ReelItemBelongsToWinning(int reelIndex, int itemIndex, int winningIndex)
{
	if (!this->ReelBelongsToWinning(reelIndex, winningIndex))
		return false;

	return this->model->lines[this->winnings.win[winningIndex].lineIndex].coordinates[reelIndex] == itemIndex;
}

bool GkGameWinningsAnalyzer::ReelItemBelongsToAnyWinning(int reelIndex, int itemIndex)
{
	for (int winningIndex = 0; winningIndex < this->winnings.count; winningIndex++)
	if (this->ReelItemBelongsToWinning(reelIndex, itemIndex, winningIndex))
		return true;

	return false;
}

void GkGameWinningsAnalyzer::InitWinningData(GameWinnings *data)
{
	data->win = new GameWinning[this->model->numLines + this->model->numScatters];
	data->count = 0;
}

void GkGameWinningsAnalyzer::DisposeWinningData(GameWinnings *data)
{
	delete[] data->win;
	data->count = 0;
}

void GkGameWinningsAnalyzer::SaveWinningData(GameWinnings *destination)
{
	destination->count = this->winnings.count;
	GkOSUtils::CopyMem(destination->win, this->winnings.win, sizeof(GameWinning) * this->winnings.count);
}

WinningDataComparisonResult GkGameWinningsAnalyzer::FindWinning(GameWinning *gameWinning)
{
	for (int i = 0; i < this->winnings.count; i++)
	{
		GameWinning *testedWinning = &this->winnings.win[i];

		if ((testedWinning->symbolIndex == gameWinning->symbolIndex) &&
			((testedWinning->lineIndex == gameWinning->lineIndex) || 
			 ((testedWinning->symbolsFlags & GkGameWinningsModel::SymbolFlag_Scatter) != 0) ||
			 (this->comparisonMode == GameWinningAnalyzerComparisonMode_IgnoreLineMatching)))
		{
			if (testedWinning->numItems == gameWinning->numItems)
				return WinningDataComparisonResult_Equivalent;
			
			if (testedWinning->numItems < gameWinning->numItems)
				return WinningDataComparisonResult_Lesser;
			
			if (testedWinning->numItems > gameWinning->numItems)
				return WinningDataComparisonResult_Greater;
		
		}
	}

	return WinningDataComparisonResult_Different;
}

WinningDataComparisonResult GkGameWinningsAnalyzer::CompareWith(GameWinnings *data)
{
	bool containsLesserWin = false,
		 containsGreaterWin = false;

	int numMatchingWins = 0;

	for (int winIndex = 0; winIndex < data->count; winIndex++)
	{
		GameWinning *winning = &data->win[winIndex];

		WinningDataComparisonResult camparedToData = this->FindWinning(winning);

		if (camparedToData == WinningDataComparisonResult_Lesser)
			containsLesserWin = true;

		if (camparedToData == WinningDataComparisonResult_Greater)
			containsGreaterWin = true;

		if (camparedToData != WinningDataComparisonResult_Different)
			numMatchingWins++;
	}

	bool containsAllDataWins = numMatchingWins == data->count,
		 containsOtherWins = numMatchingWins < this->winnings.count;

	if (containsAllDataWins && !containsOtherWins && !containsLesserWin && !containsGreaterWin)
		return WinningDataComparisonResult_Equivalent;

	if (containsAllDataWins && (containsOtherWins || containsGreaterWin)  && !containsLesserWin)
		return WinningDataComparisonResult_Greater;

	if ((!containsAllDataWins || containsLesserWin) && !containsOtherWins && !containsGreaterWin)
		return WinningDataComparisonResult_Lesser;


	return WinningDataComparisonResult_Different;
}

int GkGameWinningsAnalyzer::GetWinningReelsCount()
{
	int result = 0;

	for (int reelIndex = 0; reelIndex < this->model->numReels; reelIndex++)
		if (this->ReelBelongsToAnyWinning(reelIndex))
			result++;

	return result;
}

int GkGameWinningsAnalyzer::GetWinningReelsCount(int winningIndex, int startReel, int numReels)
{
	int result = 0;

	if (numReels == -1)
		numReels = this->model->numReels;

	for (int reelIndex = startReel; reelIndex < startReel + numReels; reelIndex++)
		if (this->ReelBelongsToWinning(reelIndex, winningIndex))
			result++;

	return result;
}

int GkGameWinningsAnalyzer::GetWinningItemReelIndex(int winningItemIndex)
{
	for (int reelIndex = 0; reelIndex < this->model->numReels; reelIndex++)
	{
		if (this->ReelBelongsToAnyWinning(reelIndex))
		{
			if (winningItemIndex == 0)
				return reelIndex;

			winningItemIndex--;
		}
	}

	return -1;
}

int GkGameWinningsAnalyzer::GetWinningItemReelIndex(int winningItemIndex, int winningIndex)
{
	for (int reelIndex = 0; reelIndex < this->model->numReels; reelIndex++)
	{
		if (this->ReelBelongsToWinning(reelIndex, winningIndex))
		{
			if (winningItemIndex == 0)
				return reelIndex;

			winningItemIndex--;
		}
	}

	return -1;
}

int GkGameWinningsAnalyzer::GetHighestWinning()
{
	if (this->winnings.count == 0)
		return -1;

	int result = 0;
	for (int winIndex = 1; winIndex < this->winnings.count; winIndex++)
		if (this->CompareWinning(winIndex, result) > 0)
			result = winIndex;

	return result;
}