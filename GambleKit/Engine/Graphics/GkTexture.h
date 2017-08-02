#pragma once

#include "..\GkDefinitions.h"

#include "GkImage.h"
#include "..\Base\GkBase.h"

class GKDLLCLASS GkTexture :
	public GkBase
{
public:
	static bool powerOfTwo;

	unsigned int handle;

	int width, height,
		textureWidth, textureHeight;

	GkTexture::GkTexture(void);
	GkTexture::~GkTexture(void);

	void GkTexture::LoadFromImage(GkImage* image);
	void GkTexture::LoadFromImage(GkImage* image, int x, int y, int width, int height);
	void GkTexture::LoadFromFile(char* fileName);

	static int GkTexture::GetPowerOfTwo(int value);
};