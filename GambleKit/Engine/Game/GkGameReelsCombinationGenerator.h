#pragma once

#include "..\GkDefinitions.h"

#include "..\Base\GkBaseReelMap.h"
#include "GkGameReelModel.h"

struct GameReelsCombinationGeneratorReel
{
	GkGameReelModel *model;
	bool blocked;
};

class GKDLLCLASS GkGameReelsCombinationGenerator
	: public GkBaseReelMap
{
public:
	
	GameReelsCombinationGeneratorReel *reels;

	GkGameReelsCombinationGenerator(int numReels, int numReelItems);
	~GkGameReelsCombinationGenerator(void);

	void SetReelModel(int reelIndex, GkGameReelModel *reelModel, bool blocked = false);

	void GenerateReel(int reelIndex);
	void GenerateCombination();
	void GenerateCombinationZero();
};

