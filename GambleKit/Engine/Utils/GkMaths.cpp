//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	GambleKit Copyright (C) 2014 Ivan Donev
//
//	This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//	contact: gamblekit@gmail.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "GkMaths.h"

#include <stdlib.h>
#include <time.h>
#include <cmath>

GkMaths::GkMaths(void)
{
}


GkMaths::~GkMaths(void)
{
}

int GkMaths::Min(int a, int b)
{
	if (a < b)
		return a;

	return b;
}

int GkMaths::Max(int a, int b)
{
	if (a > b)
		return a;

	return b;
}

float GkMaths::Max(float a, float b)
{
	if (a > b)
		return a;

	return b;
}

int GkMaths::Trim(int x, int min, int max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

int GkMaths::Sign(int x)
{
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

float GkMaths::Sign(float x)
{
	return x > 0 ? 1.0f : x < 0 ? -1.0f : 0.0f;
}

float GkMaths::Abs(float x)
{
	return x >= 0 ? x : -x;
}

int GkMaths::Abs(int x)
{
	return x >= 0 ? x : -x;
}

int GkMaths::Random(int range)
{
	return rand() % range;
}


void GkMaths::Randomize()
{
	srand((unsigned int)time(NULL));
}