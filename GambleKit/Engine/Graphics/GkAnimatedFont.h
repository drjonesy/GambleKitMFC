#pragma once

#include "..\GkDefinitions.h"

#include "..\Base\GkBase.h"
#include "GkAnimatedTexture.h"

class GKDLLCLASS GkAnimatedFont :
	public GkBase
{
public:
	wchar_t *charMap;
	int numLetters, numFrames;

	float framesPerSecond;

	GkAnimatedTexture **letterTextures;

	GkAnimatedFont::GkAnimatedFont(const wchar_t *charMap, int numFrames, float framesPerSecond = 25.0f);
	GkAnimatedFont::~GkAnimatedFont(void);

	void GkAnimatedFont::LoadAnimatedLetter(int letterIndex, void *data, int dataSize);
};

