#pragma once

#include "..\GkDefinitions.h"

#include <iostream>
#include <vector>

#include "..\Graphics\GkGraphicsDevice.h"
#include "..\Graphics\GkAnimatedTexture.h"
#include "..\Graphics\GkAnimatedFont.h"
#include "..\Sound\GkSoundDevice.h"
#include "..\Sound\GkSoundSource.h"
#include "GkGameTimer.h"
#include "GkGamePage.h"
#include "..\Base\GkBase.h"

using namespace std;

class GKDLLCLASS GkGamePage;

struct ResourcePoolEntry
{
	GkBase* resource;
	wchar_t* id;

	ResourcePoolEntry::ResourcePoolEntry(GkBase *resource, const wchar_t *id);
};

class GKDLLCLASS GkGame
{
public:
	GkGraphicsDevice *graphicsDevice;
	GkSoundDevice *soundDevice;

	GkGameTimer *timer;

	vector<GkGamePage*> gamePages;
	GkGamePage *currentGamePage;

	vector<ResourcePoolEntry> resourcePool;

	GkGame::GkGame(GkGraphicsDevice *graphicsDevice, GkSoundDevice *soundDevice);
	virtual GkGame::~GkGame(void);
	
	void SetPage(GkGamePage *page);
	GkAnimatedTexture* GkGame::AddAnimatedTexture(GkAnimatedTexture *animatedTexture, const wchar_t* id = 0);
	void* GkGame::GetResource(const wchar_t* id);
	bool GkGame::ReleaseResource(const wchar_t* id);
	GkAnimatedFont* GkGame::AcquireAnimatedFontFromMultipleFiles(const wchar_t* virtualFileName, const wchar_t *charMap, int numFrames = 1);
	GkAnimatedTexture* GkGame::AcquireAnimatedTexture(const wchar_t* virtualFileName, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
	GkAnimatedTexture* GkGame::AcquireAnimatedTextureFromMultipleFiles(const wchar_t* virtualFileName, float framesPerSecond = 25.0f);
	GkSoundSource* GkGame::AcquireSoundSource(const wchar_t* virtualFileName);
	GkGamePage* GkGame::AddGamePage(GkGamePage *gamePage);
	void GkGame::RemoveGamePage(GkGamePage *gamePage);
	virtual void GkGame::MouseDown(int x, int y);
	virtual void GkGame::MouseUp(int x, int y);
	virtual void GkGame::NewFrame();
	virtual void GkGame::Process();
	virtual void GkGame::Render();
};

