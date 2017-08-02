#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\..\GkGameEntity.h"
#include "..\..\Entities\GkGameButton.h"

class GKDLLCLASS GkGameButtonsBinding :
	public GkGameEntity
{
public:
	int numGameButtons;
	GkGameButton **gameButtons;

	bool enabled;

	bool pushedValue, clickedValue;

	GkGameButtonsBinding(int numGameButtons, ...);
	~GkGameButtonsBinding(void);

	void SyncPush(int syncButtonIndex);
	void SyncClick(int syncButtonIndex);

	void Process(GkGame *game);
};

