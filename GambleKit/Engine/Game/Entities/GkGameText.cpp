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

#include "GkGameText.h"

#include "..\GkGame.h"
#include "..\..\Utils\GkStrings.h"

GkGameText::GkGameText(GkAnimatedFont *animatedFont, int x, int y, int characterSpacing, int textAlignment, int maxWidth, float framesPerSecond)
	: GkGameEntity()
{
	this->animatedFont = animatedFont;

	this->x = x;
	this->y = y;
	this->characterSpacing = characterSpacing;
	this->maxWidth = maxWidth;
	this->textAlignment = textAlignment;
	this->visible = animatedFont != NULL;
	this->active = animatedFont != NULL;
	this->loop = animatedFont != NULL;

	this->framesPerSecond = framesPerSecond != 0.0f ? framesPerSecond : animatedFont != 0 ? animatedFont->framesPerSecond : 0.0f;
	this->currentFrame = 0.0f;

	this->text = 0;
	this->textTextureIndices = 0;
	this->textLength = 0;
}


GkGameText::~GkGameText(void)
{
	if (this->text != 0)
		delete[] this->text;

	if (this->textTextureIndices != 0)
		delete[] this->textTextureIndices;
}

bool GkGameText::SetText(const wchar_t *text)
{
	if ((this->text != 0) && (GkStrings::Compare(this->text, text) == 0))
		return false;

	if (this->text != 0)
		delete[] this->text;

	if (this->textTextureIndices != 0)
		delete[] this->textTextureIndices;

	this->text = GkStrings::Copy(text);
	this->textLength = GkStrings::GetLength(this->text);
	this->textTextureIndices = new int[this->textLength];

	this->textWidth = 0;
	for (int i = 0; i < this->textLength; i++)
	{
		this->textTextureIndices[i] = GkStrings::SubChar(this->animatedFont->charMap, this->text[i]);

		if (this->textTextureIndices[i] != -1)
		{
			this->textWidth += this->animatedFont->letterTextures[this->textTextureIndices[i]]->frames[0]->width + this->characterSpacing;
		}
	}

	this->textWidth -= this->characterSpacing;

	return true;
}

void GkGameText::Process(GkGame *game)
{
	if (!(this->active && this->visible))
		return;

	this->currentFrame += this->framesPerSecond * game->timer->tickTime;
	if (this->currentFrame > this->animatedFont->letterTextures[0]->numFrames)
	{
		if (this->loop)
		{
			int intFrame = (int)this->currentFrame;
			float fracFrame = this->currentFrame - intFrame;
			this->currentFrame = intFrame % this->animatedFont->letterTextures[0]->numFrames + fracFrame;
		}
		else
		{
			this->currentFrame = (float)(this->animatedFont->letterTextures[0]->numFrames - 1);
			this->active = false;
		}
	}
}

void GkGameText::Draw(GkGame *game)
{
	if (!this->visible)
		return;

	if ((this->maxWidth == 0) || (this->textWidth <= this->maxWidth))
	{
		int x = this->textAlignment == GkGameText::GameTextAlignment_Right ? this->x : this->textAlignment == GkGameText::GameTextAlignment_Center ? this->x - this->textWidth / 2 : this->x - this->textWidth;

		for (int i = 0; i < this->textLength; i++)
		{
			int letterTextureIndex = this->textTextureIndices[i];

			if (letterTextureIndex != -1)
			{
				GkTexture *letterTexture = this->animatedFont->letterTextures[letterTextureIndex]->frames[(int)this->currentFrame];
			
				game->graphicsDevice->Draw(x, this->y, letterTexture);

				x += letterTexture->width + this->characterSpacing;
			}
		}
	}
	else
	{
		float scalingFactor = (float)this->maxWidth / (float)this->textWidth;

		float x = this->textAlignment == GkGameText::GameTextAlignment_Right ? this->x : this->textAlignment == GkGameText::GameTextAlignment_Center ? this->x - (float)(scalingFactor * (float)this->textWidth / 2.0f) : this->x - (float)(scalingFactor * (float)this->textWidth);
		int y = this->y + (int)(((1.0f - scalingFactor) * (float)this->animatedFont->letterTextures[0]->frames[0]->height) / 2.0f);

		for (int i = 0; i < this->textLength; i++)
		{
			int letterTextureIndex = this->textTextureIndices[i];

			if (letterTextureIndex != -1)
			{
				GkTexture *letterTexture = this->animatedFont->letterTextures[letterTextureIndex]->frames[(int)this->currentFrame];
			
				game->graphicsDevice->Draw((int)x, y, (int)(letterTexture->width * scalingFactor), (int)(letterTexture->height * scalingFactor), 0, 0, letterTexture->width, letterTexture->height, letterTexture);

				x += scalingFactor * (letterTexture->width + this->characterSpacing);
			}
		}
	}
}

