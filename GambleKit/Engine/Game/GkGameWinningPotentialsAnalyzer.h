#pragma once

#include "..\GkDefinitions.h"

#include "GkGameWinningsAnalyzer.h"

class GKDLLCLASS GkGameWinningPotentialsAnalyzer :
	public GkGameWinningsAnalyzer
{
public:
	GkGameWinningPotentialsAnalyzer(GkGameWinningsModel *model);
	virtual ~GkGameWinningPotentialsAnalyzer(void);

	virtual GameWinning GetSymbolWinning(int symbolIndex, int lineIndex);
	virtual GameWinning GetScatterWinning(int symbolIndex);

	bool ReelBelongsToWinningAndSymbol(int reelIndex, int winningIndex);
	bool ContainsActualWin(int winningIndex);
};
