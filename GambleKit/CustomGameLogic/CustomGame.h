#pragma once

#include "..\Engine\GambleKit.h"

//#include "Settings.h"
//#include "Dictionary.h"

struct LogRecordGamePlay
{
	DateTime time;
	int bet;
	int winning;
};

class MainPage;
class MainMenuPage;

class CustomGame :
	public GkGame
{
public:
	static const wchar_t *dataFileName, *settingsFileName;
	static const wchar_t *screenShotsFileExp;

	bool isPayingCredits;
	int creditsPayed;

	//Settings *settings;

	GkLogFile<LogRecordGamePlay> *logGamePlay;
	
	MainPage *mainPage;

	MainMenuPage *mainMenuPage;
	
	CustomGame(GkGraphicsDevice *graphicsDevice, GkSoundDevice *soundDevice);
	~CustomGame(void);
};

