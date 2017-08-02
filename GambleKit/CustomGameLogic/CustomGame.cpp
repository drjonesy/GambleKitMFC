#include "CustomGame.h"

#include "Pages\MainPage.h"

const wchar_t *CustomGame::settingsFileName = L"..\\Settings.dat";

const wchar_t *CustomGame::screenShotsFileExp = L"..\\Logging\\Plays\\play{0}.jpg";

CustomGame::CustomGame(GkGraphicsDevice *graphicsDevice, GkSoundDevice *soundDevice) :
	GkGame(graphicsDevice, soundDevice)
{
	this->logGamePlay = new GkLogFile<LogRecordGamePlay>(L"..\\Logging\\GamePlay.log");

	//this->settings = new Settings(CustomGame::settingsFileName, this);
	
	this->AddGamePage(new MainPage(this));
}


CustomGame::~CustomGame(void)
{
	delete this->logGamePlay;

	//delete this->settings;
}
