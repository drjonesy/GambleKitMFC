#pragma once

#include "..\GkDefinitions.h"

class GKDLLCLASS GkGame;

class GKDLLCLASS GkGameEntity
{
public:
	GkGameEntity::GkGameEntity();
	virtual GkGameEntity::~GkGameEntity(void);

	virtual void GkGameEntity::MouseDown(int x, int y);
	virtual void GkGameEntity::MouseUp(int x, int y);
	virtual void GkGameEntity::Process(GkGame *game);
	virtual void GkGameEntity::Draw(GkGame *game);
	virtual bool GkGameEntity::IsMouseOver(int x, int y);
	virtual void GkGameEntity::OnMouseClicked();
	virtual bool GkGameEntity::GetBoundaries(int *x1, int *y1, int *x2, int*y2);
};