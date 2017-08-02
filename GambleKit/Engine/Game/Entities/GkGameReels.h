#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameEntity.h"
#include "GkGameAnimation.h"
#include "..\..\Base\GkBaseReelMap.h"
#include "..\GkGameReelsCombinationGenerator.h"

class GKDLLCLASS GkGameTimer;

enum GameReelState
{
	Idle = 0,
	PreStarting = 1,
	Starting = 2,
	Spinning = 3,
	Stopping = 4,
	PostStopping = 5,
	Blocked = 6,
	Undefined = 7
};

struct GkGameReelInfo
{
	GameReelState state, stateOld, stateChange;
	float position, speed;
	int reelMapSetIndex; 

	GkGameAnimation **animations;
};

class GKDLLCLASS GkGameReels :
	public GkGameEntity,
	public GkBaseReelMap
{
private:
	GameReelState stateOld;
	long long stopInitiatedTime, reelingTime, idleTime;

	// GkGameReel related methods
	GkAnimatedTexture *GkGameReels::Reel_GetNewSymbol(int reelIndex);
	void GkGameReels::Reel_ProcessState(int reelIndex, GkGameTimer *gameTimer);
	void GkGameReels::Reel_ProcessPosition(int reelIndex, GkGame *game);
	bool GkGameReels::Reel_Start(int reelIndex);
	bool GkGameReels::Reel_Stop(int reelIndex);

	void GkGameReels::ProcessState(GkGameTimer *gameTimer);

public:
	GkGameReelInfo *reels;

	
	GameReelState state,		// GkGameReels acquires certain state when all of its reels reach that state
				  stateChange;  // Gives the state that reels have just obtained in the last frame

	int numSymbols, numReelAnimations;

	int x, y, width, height; 

	float speed, // items per second
		  acceleration, // items per second^2
		  stoppingTime, // seconds
		  preStartOffset, postStopOffset, // number of items
		  preStartSpeed,  postStopSpeed; // items per second
	
	GkAnimatedTexture **symbols;

	GkGameReelsCombinationGenerator *itemGenerator;

	GkGameReels::GkGameReels(int numReels, int numReelItems, int numSymbols, int width, int height);
	GkGameReels::~GkGameReels(void);

	void GkGameReels::SetItemGenerator(GkGameReelsCombinationGenerator *itemGenerator);
	void GkGameReels::Process(GkGame *game);
	void GkGameReels::Draw(GkGame *game);
	void GkGameReels::ApplyReelMap(const int *fullItemMap = 0);
	bool GkGameReels::Start();
	bool GkGameReels::Stop();
	bool GkGameReels::ItemsAnimationsPlaying();
	void GkGameReels::StopItemsAnimations();
	float GkGameReels::GetReelingTime();
	float GkGameReels::GetIdleTime();
	GameReelState GkGameReels::GetGeneralState();
};

