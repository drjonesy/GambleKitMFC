#pragma once

#include "..\GkDefinitions.h"

class GKDLLCLASS GkMaths
{
public:
	GkMaths(void);
	~GkMaths(void);

	static int GkMaths::Trim(int x, int min, int max);
	static int GkMaths::Min(int a, int b);
	static int GkMaths::Max(int a, int b);
	static float GkMaths::Max(float a, float b);
	static int GkMaths::Sign(int x);
	static float GkMaths::Sign(float x);
	static float GkMaths::Abs(float x);
	static int GkMaths::Abs(int x);

	static int GkMaths::Random(int n);
	static void GkMaths::Randomize();
};

