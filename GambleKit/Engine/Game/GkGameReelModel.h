#pragma once

#include "..\GkDefinitions.h"

class GKDLLCLASS GkGameReelModel
{
public:
	int numItems;
	int *items;

	GkGameReelModel(int numItems);
	virtual ~GkGameReelModel(void);

	virtual int GetSymbol() = 0;
	virtual void GenerateReel() = 0;
	virtual void GenerateReelZero() = 0;
};

