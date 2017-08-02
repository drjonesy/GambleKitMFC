#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameEntity.h"
#include "..\..\Graphics\GkAnimatedFont.h"

class GKDLLCLASS GkGameText : 
	public GkGameEntity
{
public:
	static const int GameTextAlignment_Right = 0,
					 GameTextAlignment_Center = 1,
					 GameTextAlignment_Left = 2;

	GkAnimatedFont *animatedFont;
	wchar_t *text;
	int textLength, textWidth;
	int *textTextureIndices;

	int x, y;
	bool visible, active, loop;
	float framesPerSecond, currentFrame;
	int textAlignment;
	int characterSpacing;
	int maxWidth;


	GkGameText::GkGameText(GkAnimatedFont *animatedFont, int x = 0, int y = 0, int characterSpacing = 0, int textAlignment = GameTextAlignment_Right, int maxWidth = 0, float framesPerSecond = 0.0f);
	GkGameText::~GkGameText(void);

	bool GkGameText::SetText(const wchar_t *text);
	void GkGameText::Process(GkGame *game);
	void GkGameText::Draw(GkGame *game);
};

