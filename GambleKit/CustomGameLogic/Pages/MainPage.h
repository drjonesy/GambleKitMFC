#pragma once

#include "..\CustomGame.h"

class MainPage :
	public GkGamePage
{
public:

	static const int ReelItem_Cherry	= 0,
					 ReelItem_Melon		= 1,
					 ReelItem_Bell		= 2,
					 ReelItem_7			= 3;

	//only update: added pointer *btnBetAll
	GkGameClickButton *btnStart, *btnStop, *btnBet, *btnLines, *btnBetAll;

	GkGameText *textCredits, *textWinning, *textBet, *textLines;

	GkGameAnimation *lineAnimations[5];

	GkGameReels *reels;

	GkGameReelsCombinationGenerator *combinationsGenerator;
	GkGameWinningsModel *winningsModel;
	GkGameWinningsAnalyzer *winningsAnalyzer;

	int bet, lines, winning, credits;

	GkSoundSource *sndReels, *sndReelStop;

	MainPage(CustomGame *game);
	~MainPage(void);

	void Process(GkGame *game);
};

