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

#include "GkAnimatedTexture.h"

GkAnimatedTexture::GkAnimatedTexture(void)
{
	this->numFrames = 0;
}

GkAnimatedTexture::GkAnimatedTexture(void* data, unsigned int dataSize, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	this->numFrames = 0;

	this->LoadFromMemory(data, dataSize, columnFrames, rowFrames, numFrames, framesPerSecond, animatedTexture_LoadOrder);
}

GkAnimatedTexture::GkAnimatedTexture(char *fileName, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	this->numFrames = 0;

	this->LoadFromFile(fileName, columnFrames, rowFrames, numFrames, framesPerSecond, animatedTexture_LoadOrder);
}

GkAnimatedTexture::~GkAnimatedTexture(void)
{
	this->Free();
}

void GkAnimatedTexture::Free()
{
	if (this->numFrames == 0)
		return;

	for (int i = 0; i < this->numFrames; i++)
	{
		delete this->frames[i];
	}

	delete this->frames;

	this->numFrames = 0;
}

bool GkAnimatedTexture::LoadFromImage(GkImage *image, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	this->Free();

	this->numFrames = numFrames != 0 ? numFrames : columnFrames * rowFrames;
	this->framesPerSecond = framesPerSecond;

	this->frames = new GkTexture*[this->numFrames];

	int frameWidth = image->width / columnFrames;
	int frameHeight = image->height / rowFrames;
	
	int iMax, jMax;
	if (animatedTexture_LoadOrder == GkAnimatedTexture::LoadOrder_Vertical)
	{
		jMax = columnFrames;
		iMax = rowFrames;
	}
	else
	{
		jMax = rowFrames;
		iMax = columnFrames;
	}

	int frameIndex = 0;
	for (int j = 0; j < jMax; j++)
	{
		for (int i = 0; i < iMax; i++)
		{
			int x, y;
			if (animatedTexture_LoadOrder == GkAnimatedTexture::LoadOrder_Vertical)
			{
				x = j;
				y = i;
			}
			else
			{
				x = i;
				y = j;
			}

			GkTexture *frame = new GkTexture();
			frame->LoadFromImage(image, x * frameWidth, y * frameHeight, frameWidth, frameHeight); 

			frames[frameIndex++] = frame;
		}
	}

	return true;
}

bool GkAnimatedTexture::LoadFromMemory(void* data, unsigned int dataSize, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	GkImage image;
	image.LoadFromMemory(data, dataSize);

	this->LoadFromImage(&image, columnFrames, rowFrames, numFrames, framesPerSecond, animatedTexture_LoadOrder);

	return true;
}

bool GkAnimatedTexture::LoadFromFile(char *fileName, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	GkImage image;
	image.LoadFromFile(fileName);

	this->LoadFromImage(&image, columnFrames, rowFrames, numFrames, framesPerSecond, animatedTexture_LoadOrder);

	return true;
}