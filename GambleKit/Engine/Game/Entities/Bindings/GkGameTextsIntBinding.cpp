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

#include "GkGameTextsIntBinding.h"

#include "..\..\..\Utils\GkStrings.h"

GkGameTextsIntBinding::GkGameTextsIntBinding(int* valueAddress, int numGameTexts, ...)
	: GkGameEntity()
{
	this->valueAddress = valueAddress;

	this->numGameTexts = numGameTexts;

	GkGameText **parameters = (GkGameText **)((int)&numGameTexts + 4);
	this->gameTexts = new GkGameText*[this->numGameTexts];
	for (int i = 0; i < this->numGameTexts; i++)
		this->gameTexts[i] = parameters[i];
}


GkGameTextsIntBinding::~GkGameTextsIntBinding(void)
{
	delete[] this->gameTexts;
}

void GkGameTextsIntBinding::Process(GkGame *game)
{
	for (int i = 0; i < this->numGameTexts; i++)
		this->gameTexts[i]->SetText(GkStrings::IntToString(*this->valueAddress).c_str());
}
