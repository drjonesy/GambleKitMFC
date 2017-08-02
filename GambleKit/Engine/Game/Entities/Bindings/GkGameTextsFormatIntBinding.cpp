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

#include "GkGameTextsFormatIntBinding.h"

#include "..\..\..\Utils\GkStrings.h"


GkGameTextsFormatIntBinding::GkGameTextsFormatIntBinding(const wchar_t *format, int numValues, int numGameTexts, ...)
{
	this->numValues = numValues;
	this->numGameTexts = numGameTexts;
	this->format = GkStrings::Copy(format);

	int **valueAddressParams = (int**)((int)&numGameTexts + 4);
	this->valueAddresses = new int*[this->numValues];
	this->values = new wchar_t*[this->numValues];
	for (int i = 0; i < this->numValues; i++)
		this->valueAddresses[i] = valueAddressParams[i];

	GkGameText **gameTextParams = (GkGameText **)((int)&valueAddressParams[this->numValues - 1] + 4);
	this->gameTexts = new GkGameText*[this->numGameTexts];
	for (int i = 0; i < this->numGameTexts; i++)
		this->gameTexts[i] = gameTextParams[i];
}


GkGameTextsFormatIntBinding::~GkGameTextsFormatIntBinding(void)
{
	delete[] this->valueAddresses;
	delete[] this->values;

	delete[] this->gameTexts;

	delete[] this->format;
}

void GkGameTextsFormatIntBinding::Process(GkGame *game)
{
	for (int i = 0; i < this->numValues; i++)
		this->values[i] = GkStrings::Copy(GkStrings::IntToString(*this->valueAddresses[i]).c_str());

	for (int gameTextIndex = 0; gameTextIndex < this->numGameTexts; gameTextIndex++)
	{
		this->gameTexts[gameTextIndex]->SetText(GkStrings::FormatArray(this->format, this->values).c_str());
	}

	for (int i = 0; i < this->numValues; i++)
		delete[] this->values[i];
}
