#pragma once

#include "..\GkDefinitions.h"

#include "GkTexture.h"

class GKDLLCLASS GkGraphicsDevice
{
public:

	static void Initialize();

	int maxTextureSize;
	bool nonPower2Textures;

	int width, height;

	GkGraphicsDevice(void);
	~GkGraphicsDevice(void);

	void Free();
	void Clear();

	void Draw(int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, GkTexture* texture);
	void Draw(int x, int y, GkTexture* texture);
	void DrawClipped(int x, int y, int clipX, int clipY, int clipWidth, int clipHeight, GkTexture* texture);
	void DrawString(int x, int y, const char *text);
	void DrawString(int x, int y, const wchar_t *text);
	void SetViewport(int x, int y, int width, int height);

	bool CaptureScreen(const wchar_t *fileName, int width, int height);
};