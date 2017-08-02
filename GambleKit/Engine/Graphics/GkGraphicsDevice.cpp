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

#include "GkGraphicsDevice.h"

#include "..\Utils\GkMaths.h"

#include <iostream>

#include <GL\GLUt.h>
#include <string.h>

void GkGraphicsDevice::Initialize()
{
}

GkGraphicsDevice::GkGraphicsDevice(void)
{
	this->SetViewport(0, 0, 640, 480);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_REPLACE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);
	this->nonPower2Textures = strstr((char*)glGetString(GL_EXTENSIONS),"GL_ARB_texture_non_power_of_two") != 0;

	GkTexture::powerOfTwo = !this->nonPower2Textures;
}


GkGraphicsDevice::~GkGraphicsDevice(void)
{
}


void GkGraphicsDevice::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GkGraphicsDevice::Draw(int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, GkTexture* texture)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, this->width, this->height, 0, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(destX, destY, 0);
	glScalef(destWidth, destHeight, 1);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(srcX / (float)texture->textureWidth, srcY / (float)texture->textureHeight, 0.0f);
	glScalef(srcWidth / (float)texture->textureWidth, srcHeight / (float)texture->textureHeight, 1.0f);
	
	glBindTexture(GL_TEXTURE_2D, texture->handle);

	GLint filtering = (destWidth != srcWidth) || (destHeight != srcHeight) ? GL_LINEAR : GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();
}

void GkGraphicsDevice::Draw(int x, int y, GkTexture* texture)
{
	this->Draw(x, y, texture->width, texture->height, 0, 0, texture->width, texture->height, texture);
}

void GkGraphicsDevice::DrawClipped(int x, int y, int clipX, int clipY, int clipWidth, int clipHeight, GkTexture* texture)
{
	int clipLeft = clipX - x;
	int clipTop = clipY - y;

	int destX = clipLeft > 0 ? clipX : x;
	int destY = clipTop > 0 ? clipY : y;
	int width = GkMaths::Min(texture->width - GkMaths::Max(clipLeft, 0), GkMaths::Min(clipX + clipWidth - destX, texture->width));
	int height = GkMaths::Min(texture->height - GkMaths::Max(clipTop, 0), GkMaths::Min(clipY + clipHeight - destY, texture->height));

	if ((width <= 0) || (height <= 0))
		return;

	int srcX = clipLeft > 0 ? clipLeft : 0;
	int srcY = clipTop > 0 ? clipTop : 0;

	this->Draw(destX, destY, width, height, srcX, srcY, width, height, texture);
}

void GkGraphicsDevice::DrawString(int x, int y, const char *text)
{
	

	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glRasterPos2i(x, y);

	glDisable(GL_TEXTURE_2D);
	
	for (int i = 0; text[i] != 0; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
}

void GkGraphicsDevice::DrawString(int x, int y, const wchar_t *text)
{
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glRasterPos2i(x, y);

	glDisable(GL_TEXTURE_2D);

	for (int i = 0; text[i] != 0; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);

	glEnable(GL_TEXTURE_2D);
}

void GkGraphicsDevice::SetViewport(int x, int y, int width, int height)
{
	this->width = width;
	this->height = height;

	glViewport(x, y, width, height);

	glEnable(GL_TEXTURE_2D);
}

bool GkGraphicsDevice::CaptureScreen(const wchar_t *fileName, int width, int height)
{
	int length = std::wcslen(fileName);
	char *cFileName = new char[length + 1];
	std::wcstombs(cFileName, fileName, length);
	cFileName[length] = 0;

	int dataSize = this->width * this->height * 4;
	char *data = new char[dataSize];

	glReadPixels(0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	ilTexImage(this->width, this->height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, data);
	iluImageParameter(ILU_FILTER, ILU_BILINEAR);
	iluScale(width, height, 1);
	ilSetInteger(IL_JPG_QUALITY, 50);
	ilEnable(IL_FILE_OVERWRITE);
	bool result = ilSaveImage(cFileName) != 0;
	ilDeleteImages(1, &imageId);

	delete[] data;
	delete[] cFileName;

	return result;
}
