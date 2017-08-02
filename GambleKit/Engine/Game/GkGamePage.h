#pragma once

#include "..\GkDefinitions.h"

#include <vector>

#include "GkGameEntity.h"

class GKDLLCLASS GkGame;

class GKDLLCLASS GkGamePage
{
public:
	std::vector<GkGameEntity*> entities;

	GkGamePage::GkGamePage();
	virtual GkGamePage::~GkGamePage(void);

	virtual void OnShow(GkGamePage *previous);
	virtual void OnHide(GkGamePage *next);

	GkGameEntity* GkGamePage::AddEntity(GkGameEntity *entity);
	GkGameEntity* GkGamePage::InsertEntity(int position, GkGameEntity *entity);
	bool GkGamePage::RemoveEntity(GkGameEntity *entity);
	virtual void GkGamePage::Process(GkGame *game);
	virtual void GkGamePage::Render(GkGame *game);
};

