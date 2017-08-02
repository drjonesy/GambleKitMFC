#pragma once

#include "..\GkDefinitions.h"

class GKDLLCLASS GkGameTimer
{
public:
	long long counterTime, counterFrequency;

	float tickTime;

	GkGameTimer(void);
	~GkGameTimer(void);

	void GkGameTimer::Reset();
	float GkGameTimer::Tick();

	static long long GetSample();
	static float GetSampleTime(long long sample);
};

