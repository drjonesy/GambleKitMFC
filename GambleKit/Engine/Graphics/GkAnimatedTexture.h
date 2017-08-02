#pragma once

#include "..\GkDefinitions.h"

#include "..\Base\GkBase.h"
#include "GkTexture.h"

class GKDLLCLASS GkAnimatedTexture :
	public GkBase
{
public:
	static const int LoadOrder_Horizontal = 0,
					 LoadOrder_Vertical = 1;

	GkTexture **frames;
	int numFrames;
	float framesPerSecond;

	GkAnimatedTexture::GkAnimatedTexture(void);
	GkAnimatedTexture::GkAnimatedTexture(void* data, unsigned int dataSize, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
	GkAnimatedTexture::GkAnimatedTexture(char *fileName, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
	GkAnimatedTexture::~GkAnimatedTexture(void);

	void GkAnimatedTexture::Free();
	bool GkAnimatedTexture::LoadFromImage(GkImage *image, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
	bool GkAnimatedTexture::LoadFromMemory(void* data, unsigned int dataSize, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
	bool GkAnimatedTexture::LoadFromFile(char *fileName, int columnFrames = 1, int rowFrames = 1, int numFrames = 0, float framesPerSecond = 25.0f, int animatedTexture_LoadOrder = GkAnimatedTexture::LoadOrder_Horizontal);
};

