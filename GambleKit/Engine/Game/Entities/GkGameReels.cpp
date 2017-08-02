//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	GambleKit Copyright (C) 2014 Ivan Donev
//
//	This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//	contact: gamblekit@gmail.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//#include <stdlib.h>

// Consider reels options:
// 1.	GkGameReel is a class, a fully visual GkGameEntity
//		GkGameReels is a collection of GkGameReel objects and manager for lines, winnings, sequential starting/stopping a reel, etc.
//		Pros. GkGameReels is lighest, GkGameReel is a fully useful GkGameEntity
//		Cons. Many settings, such as the "items" collection of AnimatedTextures, the "reelMap" and etc. must reoccur in all GkGameReel objects and the GkGameReels object, Changing a setting in GkGameReels will not "automatically" take effect until it is changed in all GkGameReel objects also
// 2.	GkGameReel is a class, and is abstract in a sense that it provides only the mechanics of a reel via a spinning position
//		GkGameReels is a collection of GkGameReel objects and aside from all the other things, also does the drawing of the reels
//		Pros. GkGameReels is relatively light, GkGameReel functionality is separated in its own class
//		Cons. GkGameReel is not useful unless used with GkGameReels. GkGameReels holds relatively little (maybe close to irrelevant for a whole class) logic and value
// 3.	GkGameReels has each reel internal and has methods processing a reel. All the logic is inside that class.
//		Pros. No GkGameReel class, no need for different objects to share or have the same information (for settings)
//		Cons. GkGameReels is more heavily loaded with code logic. Does not quite correspond to the architecture model so far.

#include "GkGameReels.h"

#include "..\..\OSSpecific\GkOSUtils.h"
#include "..\..\Utils\GkMaths.h"

#include "..\GkGame.h"
#include "..\..\Utils\GkMaths.h"


GkGameReels::GkGameReels(int numReels, int numReelItems, int numSymbols, int width, int height)
	: GkBaseReelMap(numReels, numReelItems)
{
	this->width = width;
	this->height = height;
	this->numSymbols = numSymbols;
	this->state = Idle;
	this->stateOld = Idle;
	this->stateChange = Undefined;

	this->numReelAnimations = this->numReelItems + 2;

	this->reels = new GkGameReelInfo[numReels];
	for (int i = 0; i < numReels; i++)
	{
		this->reels[i].state = Idle;
		this->reels[i].stateOld = Idle;
		this->reels[i].stateChange = Undefined;
		this->reels[i].position = 0.0f;

		this->reels[i].animations = new GkGameAnimation*[this->numReelAnimations];

		for (int itemIndex = 0; itemIndex < this->numReelAnimations; itemIndex++)
		{
			this->reels[i].animations[itemIndex] = new GkGameAnimation();
			this->reels[i].animations[itemIndex]->visible = true;
			this->reels[i].animations[itemIndex]->active = false;
			this->reels[i].animations[itemIndex]->loop = false;
		}
	}

	this->symbols = new GkAnimatedTexture*[numSymbols];

	this->speed = 20.0f;
	this->acceleration = 40.0f;
	this->stoppingTime = 1.0f;

	this->preStartOffset = 0.15f;
	this->preStartSpeed = 2.0f;
	this->postStopOffset = 0.15f;
	this->postStopSpeed = 2.0f;

	this->itemGenerator = 0;
}


GkGameReels::~GkGameReels(void)
{
	for (int i = 0; i < numReels; i++)
	{
		for (int j = 0; j < this->numReelAnimations; j++)
		{
			delete[] this->reels[i].animations[j];
		}

		delete[] this->reels[i].animations;
	}

	delete[] this->symbols;
	delete[] this->reels;
}

void GkGameReels::SetItemGenerator(GkGameReelsCombinationGenerator *itemGenerator)
{
	this->itemGenerator = itemGenerator;
}

GkAnimatedTexture *GkGameReels::Reel_GetNewSymbol(int reelIndex)
{
	if (this->itemGenerator != 0)
	{
		return this->symbols[this->itemGenerator->reels[reelIndex].model->GetSymbol()];
	}
	else
		return this->symbols[GkMaths::Random(this->numSymbols)];
}

void GkGameReels::Reel_ProcessPosition(int reelIndex, GkGame *game)
{
	GkGameReelInfo *reel = &this->reels[reelIndex];

	// Handle position ranging and shifting of symbols when position hops over 1.0
	reel->position += game->timer->tickTime * reel->speed;
	if (reel->position >= 1.0f)
	{
		int numShifts = (int)reel->position;

		reel->position -= numShifts;

		int reelShifts = GkMaths::Min(numShifts, this->numReelItems - reel->reelMapSetIndex + 1);

		for (int shiftIndex = 0; shiftIndex < reelShifts; shiftIndex++)
		{
			GkGameAnimation *rotAnimation = reel->animations[this->numReelAnimations - 1];
			for (int i = 0; i < this->numReelAnimations; i++)
			{
				GkGameAnimation *t = reel->animations[i];
				reel->animations[i] = rotAnimation;
				rotAnimation = t;
			}
		
			if (reel->state == Stopping)
			{
				if (reel->reelMapSetIndex < this->numReelItems)
					reel->animations[0]->SetAnimatedTexture(this->symbols[this->GetSymbol(reelIndex, this->numReelItems - 1 - reel->reelMapSetIndex)]);
				else
				{
					reel->animations[0]->SetAnimatedTexture(this->Reel_GetNewSymbol(reelIndex));
				}
			
				reel->reelMapSetIndex++;
			}
			else
			{
				reel->animations[0]->SetAnimatedTexture(this->Reel_GetNewSymbol(reelIndex));
			}
		}
	}

	for (int i = 0; i < this->numReelAnimations; i++)
	{
		reel->animations[i]->Process(game);
	}
}

void GkGameReels::Reel_ProcessState(int reelIndex, GkGameTimer *timer)
{
	GkGameReelInfo *reel = &this->reels[reelIndex];
	
	switch (reel->state)
	{
		case Idle:
			{
				reel->speed = 0.0f;
				reel->position = 0.0f;
					
				// User interaction expected
				break;
			}

		case PreStarting:
			{
				reel->speed = 0.0f;
				reel->position -= timer->tickTime * this->preStartSpeed;

				if (reel->position < -this->preStartOffset)
				{
					reel->position = -this->preStartOffset;
					reel->state = Starting;
				}
				break;
			}

		case Starting:
			{
				reel->speed += this->acceleration * timer->tickTime;

				if (reel->speed >= this->speed)
				{
					reel->speed = this->speed;
					reel->state = Spinning;
				}
				break;
			}

		case Spinning:
			{
				// User interaction expected
				break;
			}

		case Stopping:
			{
				if ((reel->reelMapSetIndex > this->numReelItems) && (reel->position >= this->postStopOffset))
				{
					reel->position = this->postStopOffset;

					reel->state = PostStopping;
				}
				break;
			}

		case PostStopping:
			{
				reel->speed = 0.0f;
				reel->position -= timer->tickTime * this->postStopSpeed;

				if (reel->position < 0.0f)
				{
					reel->position = 0.0f;
					reel->state = Idle;
				}
				break;
			}
	}

	reel->stateChange = reel->state != reel->stateOld ? reel->state : Undefined;
	reel->stateOld = reel->state;
}

bool GkGameReels::Reel_Start(int reelIndex)
{
	GkGameReelInfo *reel = &this->reels[reelIndex];

	if (reel->state == Idle)
	{
		reel->state = PreStarting;
		reel->reelMapSetIndex = 0;
		return true;
	}

	return false;
}

bool GkGameReels::Reel_Stop(int reelIndex)
{
	GkGameReelInfo *reel = &this->reels[reelIndex];

	if (reel->state == Spinning)
	{
		reel->state = Stopping;
		reel->reelMapSetIndex = 0;
		return true;
	}

	return false;
}

// Gives a summary state. If all reels have the same state, returns that state, otherwise returns Undefined. If a reel has a Blocked state, it's ignored, unless all reels have Blocked state, in which case Blocked is returned.
GameReelState GkGameReels::GetGeneralState()
{
	GameReelState result = Undefined;

	int reelIndex = 0;
	while (reelIndex < this->numReels)
	{
		GameReelState reelState = this->reels[reelIndex].state;
		
		if (result == Undefined)
		{
			result = reelState;
		}
		else if (result == Blocked)
		{
			if (reelState != Blocked)
			{
				result = reelState;
			}
		}
		else
		{
			if ((result != reelState) && (reelState != Blocked))
			{
				return Undefined;
			}
		}

		reelIndex++;
	}

	return result;
}

void GkGameReels::ProcessState(GkGameTimer *timer)
{
	GameReelState generalState = this->GetGeneralState();

	// Ensure each state is followed by the next for at least one frame
	switch (this->state)
	{
		case Idle:
			{
				if ((this->state != generalState) && (generalState != Undefined))
					this->state = PreStarting;

				break;
			}
		
		case PreStarting:
			{
				if ((this->state != generalState) && (generalState != Undefined))
					this->state = Starting;

				break;
			}
		
		case Starting:
			{
				if ((this->state != generalState) && (generalState != Undefined))
					this->state = Spinning;

				break;
			}
		
		case Spinning:
			{
				if ((this->state != generalState) && (generalState != Undefined))
					this->state = Stopping;

				break;
			}
		
		case Stopping:
			{
				float fullStopTime = GkMaths::Max(this->stoppingTime - this->numReelItems / this->speed - this->postStopOffset / this->postStopSpeed, 0.0f);
				float timeSinceStopInitiated = GkGameTimer::GetSampleTime(this->stopInitiatedTime);
				int reelsStopped = fullStopTime != 0 ? GkMaths::Min((int)(1 + (this->numReels - 1) * timeSinceStopInitiated / fullStopTime), this->numReels) : this->numReels;
				
				int stopCount = 0;
				for (int i = 0; i < this->numReels; i++)
				{
					this->Reel_Stop(i);

					if ((this->reels[i].state != Blocked) && (this->reels[i].state != Spinning))
						stopCount++;

					if (stopCount >= reelsStopped)
						break;
				}

				if (reelsStopped == this->numReels)
					this->state = PostStopping;
			
				break;
			}

		case PostStopping:
			{
				if ((generalState == Idle) | (generalState == Blocked))
					this->state = Idle;
				//if ((this->state != generalState) && (generalState != Undefined))
				//	this->state = Idle;

				break;
			}
		
		default:
			{
				this->state = generalState;
			}
	}

	this->stateChange = this->state != this->stateOld ? this->state : Undefined;
	this->stateOld = this->state;
}

void GkGameReels::Process(GkGame *game)
{
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		this->Reel_ProcessPosition(reelIndex, game);

		this->Reel_ProcessState(reelIndex, game->timer);
	}

	this->ProcessState(game->timer);
}

void GkGameReels::Draw(GkGame *game)
{
	int itemWidth = this->symbols[0]->frames[0]->width;
	int itemHeight = this->symbols[0]->frames[0]->height;

	float horizontalDistance = this->numReels == 1 ? itemWidth : itemWidth + (this->width - this->numReels * itemWidth) / ((float)this->numReels - 1.0f);

	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		int x = (int)(this->x + reelIndex * horizontalDistance);

		for (int itemIndex = 0; itemIndex < this->numReelAnimations; itemIndex++)
		{
			int y = (int)(this->y + (this->height - this->numReelAnimations * itemHeight) / 2 + this->reels[reelIndex].position * itemHeight + itemIndex * itemHeight);

			GkGameAnimation *reelItemAnimation = this->reels[reelIndex].animations[itemIndex];

			game->graphicsDevice->DrawClipped(x, y, this->x, this->y, this->width, this->height, reelItemAnimation->animatedTexture->frames[(int)reelItemAnimation->currentFrame]);
		}
	}
}

void GkGameReels::ApplyReelMap(const int *fullItemMap)
{
	if (fullItemMap != 0)
	{
		this->SetItems(fullItemMap);
	}


	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		this->reels[reelIndex].animations[0]->SetAnimatedTexture(this->symbols[GkMaths::Random(this->numSymbols)]);
		this->reels[reelIndex].animations[this->numReelAnimations - 1]->SetAnimatedTexture(this->symbols[GkMaths::Random(this->numSymbols)]);
	}

	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
		{
			this->reels[reelIndex].animations[itemIndex + 1]->SetAnimatedTexture(this->symbols[this->GetSymbol(reelIndex, itemIndex)]);
		}
	}
}

bool GkGameReels::Start()
{
	this->reelingTime = GkGameTimer::GetSample();
	
	bool result = true;
	if (this->state == Idle)
	{
		for (int i = 0; i < this->numReels; i++)
			 result &= this->Reel_Start(i);

		return result;
	}

	return false;
}

bool GkGameReels::Stop()
{
	if (this->state = Spinning)
	{
		this->stopInitiatedTime = GkGameTimer::GetSample();

		this->state = Stopping;
		return true;
	}

	return false;



	bool result = true;
	if (this->state == Spinning)
	{
		for (int i = 0; i < this->numReels; i++)
			 result &= this->Reel_Stop(i);

		return result;
	}

	return false;
}

bool GkGameReels::ItemsAnimationsPlaying()
{
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
		{
			if (this->reels[reelIndex].animations[itemIndex + 1]->active)
				return true;
		}
	}

	return false;
}

void GkGameReels::StopItemsAnimations()
{
	for (int reelIndex = 0; reelIndex < this->numReels; reelIndex++)
	{
		for (int itemIndex = 0; itemIndex < this->numReelItems; itemIndex++)
		{
			this->reels[reelIndex].animations[itemIndex + 1]->active = false;
			this->reels[reelIndex].animations[itemIndex + 1]->currentFrame = 0.0f;
		}
	}
}

float GkGameReels::GetReelingTime()
{
	return GkGameTimer::GetSampleTime(this->reelingTime);
}

float GkGameReels::GetIdleTime()
{
	return GkGameTimer::GetSampleTime(this->idleTime);
}
