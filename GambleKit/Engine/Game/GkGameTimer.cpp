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

#include "GkGameTimer.h"

#include "..\OSSpecific\GkOSUtils.h"

GkGameTimer::GkGameTimer(void)
{
	this->counterFrequency = GkOSUtils::GetPerformanceCounterFrequency();

	this->Reset();
}


GkGameTimer::~GkGameTimer(void)
{
}

void GkGameTimer::Reset()
{
	this->counterTime = GkOSUtils::GetPerformanceCounter();
	
	this->Tick();
}

float GkGameTimer::Tick()
{
	long long newCounterTime = GkOSUtils::GetPerformanceCounter();

	this->tickTime = (float)((double)(newCounterTime - this->counterTime) / (double)this->counterFrequency);
	this->counterTime = newCounterTime;

	return this->tickTime;
}

long long GkGameTimer::GetSample()
{
	return GkOSUtils::GetPerformanceCounter();
}

float GkGameTimer::GetSampleTime(long long sample)
{
	return (float)(((double)(GkGameTimer::GetSample() - sample)) / ((double)GkOSUtils::GetPerformanceCounterFrequency()));
}
