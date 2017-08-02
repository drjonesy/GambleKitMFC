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

#include "GkGameWinningPotentialsAnalyzer.h"

GkGameWinningPotentialsAnalyzer::GkGameWinningPotentialsAnalyzer(GkGameWinningsModel *model)
	: GkGameWinningsAnalyzer(model)
{
}


GkGameWinningPotentialsAnalyzer::~GkGameWinningPotentialsAnalyzer(void)
{
}

GameWinning GkGameWinningPotentialsAnalyzer::GetSymbolWinning(int symbolIndex, int lineIndex)
{
	GameWinning reelItem;
	reelItem.symbolIndex = symbolIndex;
	reelItem.lineIndex = lineIndex;
	reelItem.numItems = 0;
	reelItem.itemsStart = 0;

	GameWinning result = reelItem;

	GameWinningSymbolModel *symbolModel = this->model->GetSymbolModel(symbolIndex);

	bool interrupted = false;
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		int reelSymbolIndex = this->GetLineSymbol(reelIndex, lineIndex);
		GameWinningSymbolModel *reelSymbolModel = this->model->GetSymbolModel(reelSymbolIndex);

		bool match = GkGameWinningsAnalyzer::SymbolsMatch(symbolIndex, reelSymbolIndex);
		if (match || !interrupted)
		{
			if (reelItem.numItems == 0)
			{
				reelItem.itemsStart = reelIndex;
				reelItem.symbolsFlags = GkGameWinningsModel::SymbolFlag_None;
			}

			reelItem.numItems++;
			reelItem.symbolsFlags |= reelSymbolModel->flags;

			if (reelItem.numItems > result.numItems)
			{
				result = reelItem;
			}

			if (!match)
				interrupted = true;
		}
		else
		{
			if ((symbolModel->flags & GkGameWinningsModel::SymbolFlag_AnyPlace) == 0)
				break;

			reelItem.numItems = 0;

			interrupted = false;
		}
	}

	result.itemsEnd = result.itemsStart + result.numItems - 1;
	result.winningCoefficient = this->model->GetSymbolCoefficient(result.symbolIndex, result.numItems);

	return result;
}

GameWinning GkGameWinningPotentialsAnalyzer::GetScatterWinning(int symbolIndex)
{
	GameWinning result = GkGameWinningsAnalyzer::GetScatterWinning(symbolIndex);
	
	if (result.numItems < this->numReels - 1)
	{
		result.numItems++;
		result.winningCoefficient = this->model->GetSymbolCoefficient(result.symbolIndex, result.numItems);
	}

	return result;
}

bool GkGameWinningPotentialsAnalyzer::ReelBelongsToWinningAndSymbol(int reelIndex, int winningIndex)
{
	if (!GkGameWinningsAnalyzer::ReelBelongsToWinning(reelIndex, winningIndex))
		return false;

	return this->SymbolsMatch(this->winnings.win[winningIndex].symbolIndex, this->GetLineSymbol(reelIndex, this->winnings.win[winningIndex].lineIndex));
}

bool GkGameWinningPotentialsAnalyzer::ContainsActualWin(int winningIndex)
{
	if ((winningIndex < 0) || (winningIndex >= this->winnings.count))
		return false;

	int actualSequenceLength = 0;
	for (int reelIndex = this->winnings.win[winningIndex].itemsStart; reelIndex <= this->winnings.win[winningIndex].itemsEnd; reelIndex++)
	{
		if (this->ReelBelongsToWinningAndSymbol(reelIndex, winningIndex))
			actualSequenceLength++;
		else
			break;
	}

	return this->model->GetSymbolCoefficient(this->winnings.win[winningIndex].symbolIndex, actualSequenceLength) != 0;
}
