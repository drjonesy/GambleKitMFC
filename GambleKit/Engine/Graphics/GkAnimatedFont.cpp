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

#include "GkAnimatedFont.h"

#include "..\Utils\GkStrings.h"


GkAnimatedFont::GkAnimatedFont(const wchar_t *charMap, int numFrames, float framesPerSecond)
{
	this->charMap = GkStrings::Copy(charMap);
	this->numFrames = numFrames;
	this->numLetters = GkStrings::GetLength(charMap);
	this->framesPerSecond = framesPerSecond;

	this->letterTextures = new GkAnimatedTexture*[this->numLetters];
	for (int i = 0; i < this->numLetters; i++)
	{
		this->letterTextures[i] = 0;
	}
}


GkAnimatedFont::~GkAnimatedFont(void)
{
	delete[] this->charMap;
	
	for (int i = 0; i < this->numLetters; i++)
	{
		delete this->letterTextures[i];
	}
	delete[] this->letterTextures;
}

void GkAnimatedFont::LoadAnimatedLetter(int letterIndex, void *data, int dataSize)
{
	if (this->letterTextures[letterIndex] != 0)
		delete this->letterTextures[letterIndex];

	this->letterTextures[letterIndex] = new GkAnimatedTexture(data, dataSize, 1, this->numFrames, this->numFrames);
}
