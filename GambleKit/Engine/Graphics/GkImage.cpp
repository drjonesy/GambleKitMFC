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

#include "GkImage.h"

void GkImage::Initialize()
{
	ilInit();
}

GkImage::GkImage(void)
{
	ilGenImages(1, &this->handle);
}


GkImage::~GkImage(void)
{
	ilDeleteImages(1, &this->handle);
}

void GkImage::LoadFromMemory(void* data, unsigned int dataSize)
{
	ilBindImage(this->handle);
	
	ilLoadL(IL_TYPE_UNKNOWN, data, dataSize);
	this->width = ilGetInteger(IL_IMAGE_WIDTH);
	this->height = ilGetInteger(IL_IMAGE_HEIGHT);
}

void GkImage::LoadFromFile(char* fileName)
{
	ilBindImage(this->handle);
	
	ilLoadImage(fileName);
	this->width = ilGetInteger(IL_IMAGE_WIDTH);
	this->height = ilGetInteger(IL_IMAGE_HEIGHT);
}

void GkImage::CopyPixels(void* dest)
{
	ilBindImage(this->handle);
	ilCopyPixels(0, 0, 0, this->width, this->height, 1, IL_RGBA, IL_UNSIGNED_BYTE, dest);
}

void GkImage::CopyPixels(void* dest, int x, int y, int width, int height)
{
	ilBindImage(this->handle);
	ilCopyPixels(x, y, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, dest);
}

void GkImage::CopyPixels(void* dest, int x, int y, int width, int height, int destX, int destY, int destWidth, int destHeight)
{
	ilBindImage(this->handle);
	
	for (int i = 0; i < height; i++)
	{
		ilCopyPixels(x, y + i, 0, width, 1, 1, IL_RGBA, IL_UNSIGNED_BYTE, (void*)(&((int*)dest)[(destY + i) * destWidth + destX]));
	}
}
