#pragma once

#include "..\..\GkDefinitions.h"

#include "..\GkGameEntity.h"

class GKDLLCLASS GkGameWinningCounter : 
	public GkGameEntity
{
private:
	float oldValue;

	void SetValue(float value, float maxTime);

public:
	int *actualValue;

	float minSpeed, maxTime,
		  displayValue,
		  speed;

	int intDisplayValue;
	
	GkGameWinningCounter(int *actualValue, float minSpeed = 500.0f, float maxTime = 1.0f);
	~GkGameWinningCounter(void);

	void Reach(float maxTime = 0.0f);
	bool IsCounting();

	void Process(GkGame *game);
};

