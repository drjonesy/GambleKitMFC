#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\..\GkGameEntity.h"

#include "..\GkGameText.h"

class GKDLLCLASS GkGameTextsIntBinding :
	public GkGameEntity
{
public:
	int * valueAddress;

	int numGameTexts;
	GkGameText **gameTexts;

	GkGameTextsIntBinding(int* valueAddress, int numGameTexts, ...);
	~GkGameTextsIntBinding(void);

	void Process(GkGame *game);
};

