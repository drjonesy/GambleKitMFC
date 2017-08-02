#pragma once

#include "..\GkDefinitions.h"

struct GameWinningLineModel
{
	bool active;
	int *coordinates;
};

struct GameWinningSymbolModel
{
	unsigned int flags, exclusionFlags;
	int *coefficients;
};

class GKDLLCLASS GkGameWinningsModel
{
public:
	static const unsigned int SymbolFlag_None = 0,
							  SymbolFlag_Default = 1 << 0,
							  SymbolFlag_Wild = 1 << 1,
							  SymbolFlag_AnyPlace = 1 << 2, // Line sequence can start from anywhere, not only from the beginning of a line
							  SymbolFlag_IgnoreWinning = 1 << 3,
							  SymbolFlag_AtEnd = 1 << 4,
							  SymbolFlag_Scatter = 1 << 5, // If a symbol is Scatter, it has a line index of numLines and above, and its line's coordinates are set correspondingly on each CalculateWinnings()
							  SymbolFlag_Custom = 1 << 8; // For user - defined flags. User_Defined_Flag = SymbolFlag_Custom << UserFlag_Index

	int numReels, numReelItems, numChannels, selectedChannel;

	int numLines;
	GameWinningLineModel *lines;
	
	int numSymbols, numScatters;
	GameWinningSymbolModel **symbols;

	GkGameWinningsModel(int numReels, int numReelItems, int numLines, int numSymbols, int numScatters = 0, int numChannels = 1);
	~GkGameWinningsModel(void);

	void SetupLine(int lineIndex, ...);
	void SetupSymbol(int symbolIndex, int channelIndex, unsigned int symbolFlags, unsigned int symbolExclusionFlags, ...);

	int GetLineSymbolCoordinate(int reelIndex, int lineIndex);
	GameWinningSymbolModel *GetSymbolModel(int symbol);
	int GetSymbolCoefficient(int symbol, int symbolCount);
	int CompareSymbolWinnning(int symbol1, int symbol1Count, int symbol2, int symbol2Count);
};

