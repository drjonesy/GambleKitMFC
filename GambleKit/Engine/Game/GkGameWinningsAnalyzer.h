#pragma once

#include "..\GkDefinitions.h"

#include "..\Base\GkBaseReelMap.h"
#include "GkGameWinningsModel.h"

struct GameWinning
{
	int lineIndex, symbolIndex,
		itemsStart, itemsEnd, numItems,
		winningCoefficient;

	unsigned int symbolsFlags;
};

struct GameWinnings
{
	int count;
	GameWinning *win;
};

enum WinningDataComparisonResult
{
	WinningDataComparisonResult_Different,
	WinningDataComparisonResult_Equivalent,
	WinningDataComparisonResult_Greater,
	WinningDataComparisonResult_Lesser
};

enum GameWinningAnalyzerComparisonMode
{
	GameWinningAnalyzerComparisonMode_FullComparison,
	GameWinningAnalyzerComparisonMode_IgnoreLineMatching,
};

class GKDLLCLASS GkGameWinningsAnalyzer
	: public GkBaseReelMap
{
public:
	GkGameWinningsModel *model;

	GameWinnings winnings;
	int totalWinning, totalFlags;

	GameWinningAnalyzerComparisonMode comparisonMode;

	GkGameWinningsAnalyzer(GkGameWinningsModel *model);
	virtual ~GkGameWinningsAnalyzer(void);

	virtual GameWinning GetSymbolWinning(int symbolIndex, int lineIndex);
	virtual GameWinning GetScatterWinning(int symbolIndex);
	virtual void AnalyzeWinnings(GkBaseReelMap *reelMap = 0);
	virtual bool SymbolsMatch(int symbol1Index, int testSymbolIndex);
	virtual int WinningOrder(int winningIndex1, int winningIndex2);
	virtual int CompareWinning(int winningIndex1, int winningIndex2);

	void SortWinnings();
	void MultiplyWinnings(int factor);
	int GetLineSymbol(int reelIndex, int lineIndex);

	int FindWinning(int symbolIndex, int minSymbols = 0);
	bool ReelBelongsToWinning(int reelIndex, int winningIndex);
	bool ReelBelongsToAnyWinning(int reelIndex);
	bool ReelItemBelongsToWinning(int reelIndex, int itemIndex, int winningIndex);
	bool ReelItemBelongsToAnyWinning(int reelIndex, int itemIndex);
	int GetWinningReelsCount();
	int GetWinningReelsCount(int winningIndex, int startReel = 0, int numReels = -1);
	int GetWinningItemReelIndex(int winningItemIndex);
	int GetWinningItemReelIndex(int winningItemIndex, int winningIndex);
	int GetHighestWinning();

	void InitWinningData(GameWinnings *data);
	void DisposeWinningData(GameWinnings *data);
	void SaveWinningData(GameWinnings *destination);
	bool IsLessOrEqualThan(GameWinnings *data);
	WinningDataComparisonResult FindWinning(GameWinning *gameWinning);
	WinningDataComparisonResult CompareWith(GameWinnings *data);
};

