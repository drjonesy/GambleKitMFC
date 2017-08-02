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

#include "GkGameTextsDateTimeBinding.h"

#include "..\..\..\Utils\GkStrings.h"

GkGameTextsDateTimeBinding::GkGameTextsDateTimeBinding(DateTime* valueAddress, const wchar_t *formatString, int numGameTexts, ...)
	: GkGameEntity()
{
	this->valueAddress = valueAddress;
	this->formatString = GkStrings::Copy(formatString);

	this->numGameTexts = numGameTexts;

	GkGameText **parameters = (GkGameText **)((int)&numGameTexts + 4);
	this->gameTexts = new GkGameText*[this->numGameTexts];
	for (int i = 0; i < this->numGameTexts; i++)
		this->gameTexts[i] = parameters[i];
}


GkGameTextsDateTimeBinding::~GkGameTextsDateTimeBinding(void)
{
	delete[] this->formatString;
	delete[] this->gameTexts;
}

void GkGameTextsDateTimeBinding::Process(GkGame *game)
{
	for (int i = 0; i < this->numGameTexts; i++)
		this->gameTexts[i]->SetText(GkStrings::Format(this->formatString, GkStrings::IntToString(this->valueAddress->day, 10, 2).c_str(), GkStrings::IntToString(this->valueAddress->month, 10, 2).c_str(), GkStrings::IntToString(this->valueAddress->year, 10, 4).c_str(), GkStrings::IntToString(this->valueAddress->hour, 10, 2).c_str(), GkStrings::IntToString(this->valueAddress->minute, 10, 2).c_str(), GkStrings::IntToString(this->valueAddress->second, 10, 2).c_str()).c_str());
}
