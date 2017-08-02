#pragma once

#include "..\GkDefinitions.h"

#include "GkBase.h"

class GKDLLCLASS GkBaseReelMap
	: public GkBase
{
public:
	int numReels, numReelItems;

	int *reelMap;

	GkBaseReelMap(int numReels, int numReelItems);
	~GkBaseReelMap(void);

	virtual void SetSymbol(int reelIndex, int itemIndex, int symbolIndex);
	virtual int GetSymbol(int reelIndex, int itemIndex);
	void SetItems(GkBaseReelMap *reelMap, int startReel, int startItem, int numReels, int numItems);
	void SetItems(GkBaseReelMap *reelMap);
	void SetReelItems(GkBaseReelMap *reelMap, int reelIndex);
	void SetItems(const int *reelMap);
	void WriteItems(const int *reelMap);
	int	CountItems(int reelIndex, int itemIndex);
	int	CountItems(int itemIndex);
	int FindFirstSymbol(int reelIndex, int symbolIndex);
	bool ReelEquals(GkBaseReelMap *reelMap, int reelIndex);
	bool ReelsEquals(GkBaseReelMap *reelMap);
};

