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

#include "GkGameStreamingAnimation.h"

#include "..\..\Utils\GkStrings.h"
#include "..\..\OSSpecific\GkFileSystem.h"

GkGameStreamingAnimation::GkGameStreamingAnimation(const wchar_t *fileName, const wchar_t *loadExpression, int x, int y, bool freeAtEnd, float framesPerSecond, int numFrames)
{
	this->x = x;
	this->y = y;
	this->visible = false;
	this->active = true;
	this->loop = false;
	this->freeAtEnd = freeAtEnd;
	this->currentFrame = framesPerSecond >= 0.0f ? 0.0f : (float)(numFrames - 1);
	this->framesPerSecond = framesPerSecond;
	this->numFrames = numFrames;

	this->loadExpression = GkStrings::Copy(loadExpression);
	this->fileName = GkStrings::Copy(fileName);
	this->texture = 0;
}


GkGameStreamingAnimation::~GkGameStreamingAnimation(void)
{
	this->Free();

	GkStrings::Dispose(&this->fileName);
	GkStrings::Dispose(&this->loadExpression);
}

void GkGameStreamingAnimation::SetSource(const wchar_t *fileName, const wchar_t *loadExpression, float framesPerSecond, int numFrames)
{
	this->Free();

	this->active = true;
	this->loop = false;
	this->currentFrame = framesPerSecond >= 0.0f ? 0.0f : (float)(numFrames - 1);
	this->framesPerSecond = framesPerSecond;
	this->numFrames = numFrames;

	GkStrings::Dispose(&this->fileName);
	GkStrings::Dispose(&this->loadExpression);
	this->loadExpression = GkStrings::Copy(loadExpression);
	this->fileName = GkStrings::Copy(fileName);
}

void GkGameStreamingAnimation::Free()
{
	if (this->texture != 0)
	{
		delete this->texture;
		this->texture = 0;
	}
}

bool GkGameStreamingAnimation::Load(int index)
{
	void *data;
	unsigned int dataSize;

	if (GkFileSystem::LoadFile(GkStrings::Format(this->loadExpression, GkStrings::IntToString(index, 10, 5).c_str()).c_str(), &data, &dataSize))
	{
		this->Free();

		GkImage image;
		image.LoadFromMemory(data, dataSize);

		this->texture = new GkTexture();
		this->texture->LoadFromImage(&image);

		delete data;

		return true;
	}
	
	return false;
}

void GkGameStreamingAnimation::Start()
{
	this->currentFrame = framesPerSecond >= 0.0f ? 0.0f : (float)(numFrames - 1);
	this->lastRenderedFrame = (int)this->currentFrame;
	this->visible = this->Load(this->lastRenderedFrame);
	this->active = this->visible;
}

void GkGameStreamingAnimation::Process(GkGame *game)
{
	if (!(this->active && this->visible))
		return;

	this->currentFrame += this->framesPerSecond * game->timer->tickTime;
	if ((this->lastRenderedFrame != (int)this->currentFrame) || (this->texture == 0))
	{
		bool terminate = false;
		if (!this->Load((int)this->currentFrame))
		{
			if (this->loop)
			{
				this->currentFrame = this->framesPerSecond >= 0 ? 0.0f : (float)(this->numFrames - 1);
				if (!this->Load((int)this->currentFrame))
				{
					terminate = true;
				}
			}
			else
			{
				terminate = true;
			}
		}

		if (terminate)
		{
			this->currentFrame = (float)(this->lastRenderedFrame);
			this->active = false;
			this->visible = (!this->freeAtEnd) && (this->texture != 0);
		}
	}

	this->lastRenderedFrame = (int)this->currentFrame;
}

void GkGameStreamingAnimation::Draw(GkGame *game)
{
	if (!this->visible)
	{
		this->Free();
		return;
	}

	game->graphicsDevice->Draw(this->x, this->y, this->texture);
}
