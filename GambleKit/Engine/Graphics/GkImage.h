#pragma once

#include "..\GkDefinitions.h"

#include <DevIL\il.h>
#include <DevIL\ilu.h>

class GKDLLCLASS GkImage
{
public:
	ILuint handle, 
		   width, height;

	static void GkImage::Initialize();

	GkImage::GkImage(void);
	GkImage::~GkImage(void);

	void GkImage::LoadFromMemory(void* data, unsigned int dataSize);
	void GkImage::LoadFromFile(char* fileName);
	void GkImage::CopyPixels(void* dest);
	void GkImage::CopyPixels(void* dest, int x, int y, int width, int height);
	void GkImage::CopyPixels(void* dest, int x, int y, int width, int height, int destX, int destY, int destWidth, int destHeight);
};