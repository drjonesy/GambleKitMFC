#pragma once

#include "..\..\..\GkDefinitions.h"

#include "..\..\GkGameEntity.h"

#include "..\GkGameText.h"


class GKDLLCLASS GkGameTextsFormatIntBinding :
	public GkGameEntity
{
public:
	int numValues;
	int **valueAddresses;
	wchar_t **values;

	int numGameTexts;
	GkGameText **gameTexts;

	wchar_t *format;

	GkGameTextsFormatIntBinding(const wchar_t *format, int numValues, int numGameTexts, ...);
	~GkGameTextsFormatIntBinding(void);

	void Process(GkGame *game);
};

