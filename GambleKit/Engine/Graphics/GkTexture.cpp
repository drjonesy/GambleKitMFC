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

#include "GkTexture.h"

#include <GL\GLUt.h>

bool GkTexture::powerOfTwo = false;

GkTexture::GkTexture(void)
{
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, this->handle);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

GkTexture::~GkTexture(void)
{
	glDeleteTextures(1, &handle);
}

void GkTexture::LoadFromImage(GkImage* image)
{
	this->LoadFromImage(image, 0, 0, image->width, image->height);
}

void GkTexture::LoadFromImage(GkImage* image, int x, int y, int width, int height)
{
	if (GkTexture::powerOfTwo)
	{
		this->textureWidth = GkTexture::GetPowerOfTwo(width);
		this->textureHeight = GkTexture::GetPowerOfTwo(height);
	}
	else
	{
		this->textureWidth = width;
		this->textureHeight = height;
	}

	this->width = width;
	this->height = height;

	void *imageData = new char[this->textureWidth * this->textureHeight * 4];

	if (this->width != this->textureWidth)		
		image->CopyPixels(imageData, x, y, width, height, 0, 0, this->textureWidth, this->textureHeight);
	else
		image->CopyPixels(imageData, x, y, width, height);
	
	glBindTexture(GL_TEXTURE_2D, this->handle);
	//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	delete imageData;
}

void GkTexture::LoadFromFile(char* fileName)
{
	GkImage* image = new GkImage();
	image->LoadFromFile(fileName);
	this->LoadFromImage(image);
	delete image;
}

int GkTexture::GetPowerOfTwo(int value)
{
	int result = 1;
	while (result < value)
		result *= 2;

	return result;
}