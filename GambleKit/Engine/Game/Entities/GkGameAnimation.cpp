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

#include "GkGameAnimation.h"

#include "..\..\Graphics\GkImage.h"

#include "..\GkGame.h"

GkGameAnimation::GkGameAnimation(GkAnimatedTexture *animatedTexture, int x, int y, bool active, bool loop, bool visible, bool flipX, bool flipY, float framesPerSecond, int width, int height)
{
	this->SetAnimatedTexture(animatedTexture, framesPerSecond);
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->framesPerSecond = framesPerSecond != 0.0f ? framesPerSecond : animatedTexture != 0 ? animatedTexture->framesPerSecond : 0.0f;
	this->active = active && (animatedTexture != NULL);
	this->loop = loop && (animatedTexture != NULL);
	this->visible = visible && (animatedTexture != NULL);
	this->flipX = flipX;
	this->flipY = flipY;
}

GkGameAnimation::~GkGameAnimation(void)
{
}

void GkGameAnimation::SetAnimatedTexture(GkAnimatedTexture *animatedTexture, float framesPerSecond)
{
	this->animatedTexture = animatedTexture;
	this->framesPerSecond = framesPerSecond != 0.0f ? framesPerSecond : animatedTexture != 0 ? animatedTexture->framesPerSecond : 0.0f;
	this->ResetAnimation(this->active);
}

void GkGameAnimation::Process(GkGame *game)
{
	if (!(this->active && this->visible))
		return;

	if (this->flipX)
		this->flipX = true;

	this->currentFrame += this->framesPerSecond * game->timer->tickTime;

	if (this->currentFrame > this->animatedTexture->numFrames)
	{
		if (this->loop)
		{
			int intFrame = (int)this->currentFrame;
			float fracFrame = this->currentFrame - intFrame;
			this->currentFrame = intFrame % this->animatedTexture->numFrames + fracFrame;
		}
		else
		{
			this->EndAnimation();
		}
	}

	if (this->currentFrame < 0)
	{
		if (this->loop)
		{
			int intFrame = (int)this->currentFrame;
			float fracFrame = this->currentFrame - intFrame;
			this->currentFrame = (this->animatedTexture->numFrames - 1 - intFrame) % this->animatedTexture->numFrames + fracFrame;
		}
		else
		{
			this->EndAnimation();
		}
	}
}

void GkGameAnimation::Draw(GkGame *game)
{
	if (!this->visible)
		return;

	GkTexture *frame = this->animatedTexture->frames[(int)this->currentFrame];
	int width = this->width != 0 ? this->width : frame->width;
	int height = this->height != 0 ? this->height : frame->height;
	if (this->flipX)
		if (this->flipY)
			game->graphicsDevice->Draw(this->x + width, this->y + height, -width, -height, 0, 0, frame->width,frame->height, frame);
		else
			game->graphicsDevice->Draw(this->x + width, this->y, -width, height, 0, 0, frame->width, frame->height, frame);
	else
		if (this->flipY)
			game->graphicsDevice->Draw(this->x, this->y + height, width, -height, 0, 0, frame->width, frame->height, frame);
		else
			game->graphicsDevice->Draw(this->x, this->y, width, height, 0, 0, frame->width, frame->height, frame);
}

bool GkGameAnimation::GetBoundaries(int *x1, int *y1, int *x2, int*y2)
{
	try 
	{
		*x1 = this->x;
		*y1 = this->y;

		*x2 = this->x + this->animatedTexture->frames[0]->width;
		*y2 = this->y + this->animatedTexture->frames[0]->height;
	}
	catch(...)
	{
		// Exception occured 
	}

	return true;
}


void GkGameAnimation::ResetAnimation(bool start)
{
	this->currentFrame = this->framesPerSecond < 0 ? this->animatedTexture != 0 ? (float)(this->animatedTexture->numFrames - 1) : 0.0f : 0.0f;
	this->active = start;
}

void GkGameAnimation::EndAnimation()
{
	this->currentFrame = this->framesPerSecond < 0 ? 0.0f : this->animatedTexture != 0 ? (float)(this->animatedTexture->numFrames - 1) : 0.0f;
	this->active = false;
}
