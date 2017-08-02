#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\..\GkGameEntity.h"

#include "..\GkGameText.h"

#include "..\..\..\OSSpecific\GkOSUtils.h"

class GKDLLCLASS GkGameTextsDateTimeBinding :
	public GkGameEntity
{
public:
	DateTime * valueAddress;

	int numGameTexts;
	GkGameText **gameTexts;

	wchar_t *formatString;

	GkGameTextsDateTimeBinding(DateTime* valueAddress, const wchar_t *formatString, int numGameTexts, ...);
	~GkGameTextsDateTimeBinding(void);

	void Process(GkGame *game);
};

