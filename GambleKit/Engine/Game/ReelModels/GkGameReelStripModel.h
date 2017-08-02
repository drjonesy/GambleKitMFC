#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameReelModel.h"

class GKDLLCLASS GkGameReelStripModel :
	public GkGameReelModel
{
public:
	int numSymbols;
	int *symbolStrip;
	int stripPosition;

	GkGameReelStripModel(int numItems, int numSymbols, ...);
	~GkGameReelStripModel(void);

	int GetSymbol();
	void GenerateReel();
	void GenerateReelZero();
};
