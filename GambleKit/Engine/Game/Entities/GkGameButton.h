#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameEntity.h"

class GKDLLCLASS GkGameButton :
	public GkGameEntity
{
public:
	GkGameEntity *defaultFace, *selectedFace, *disabledFace;
	int x, y, width, height;

	bool visible, enabled, autoEnable, pushed;
	bool clicked, down, read;

	GkGameButton::GkGameButton(GkGameEntity *defaultFace, GkGameEntity *selectedFace = 0, GkGameEntity *disabledFace = 0, bool autoEnable = true, int x = 0, int y = 0, int width = 0, int heght = 0);
	virtual GkGameButton::~GkGameButton(void);

	virtual void GkGameButton::Down();
	virtual void GkGameButton::Up(bool click);
	virtual void GkGameButton::MouseDown(int x, int y);
	virtual void GkGameButton::MouseUp(int x, int y);
	virtual GkGameEntity* GkGameButton::GetCurrentFace();
	
	bool GkGameButton::WasDown();
	bool GkGameButton::IsClicked();
	bool GkGameButton::IsActive();
	bool GkGameButton::IsMouseOver(int x, int y);
	void GkGameButton::Process(GkGame *game);
	void GkGameButton::Draw(GkGame *game);
};

