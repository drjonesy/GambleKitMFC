#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameReelModel.h"

struct GameReelRandomModelSymbol
{
	int value, hitProbability, maxHits,
		occurances;
};

class GKDLLCLASS GkGameReelRandomModel :
	public GkGameReelModel
{
public:
	static const int UnlimitedHits = -1;

	int numSymbols;
	GameReelRandomModelSymbol *symbols;

	int overallProbability;

	GkGameReelRandomModel(int numItems, int numSymbols);
	~GkGameReelRandomModel(void);

	void SetupSymbol(int symbolIndex, int symbolValue, int hitProbability = 1, int maxHits = GkGameReelRandomModel::UnlimitedHits);
	bool ReplaceReelSymbol(int itemIndex, int symbolIndex, bool forceReplace);
	int GetSymbolIndex();

	int GetSymbol();
	void GenerateReel();
	void GenerateReelZero();
};

